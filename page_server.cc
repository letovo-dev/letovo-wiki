#include "page_server.h"

void get_page_content(std::unique_ptr<restinio::router::express_router_t<>>& router, std::shared_ptr<cp::connection_pool> pool_ptr, std::shared_ptr<restinio::shared_ostream_logger_t> logger_ptr) {
    router.get()->http_get(R"(/page/:id(\d+))", [pool_ptr, logger_ptr](auto req, auto params) {
        std::string endpoint = req->remote_endpoint().address().to_string();
        auto qrl = req->header().path();

        std::cout<<"qrl: "<<qrl<<std::endl;

        std::regex regex(R"(\d+)");
        std::smatch match;
        std::string url = {qrl.begin(), qrl.end()};
        std::cout<<"url: "<<url<<std::endl;
        int postid = 0, i = 0;
        while (std::regex_search(url, match, regex)) {
            std::cout<<"match: "<<match.str()<< ' '<<std::stoi(match.str())<<std::endl;
            postid += (std::stoi(match.str()));
            url = match.suffix().str();
        }
        std::cout<<"postid: "<<postid<<std::endl;

        if (postid <= 0) {
            return req->create_response(restinio::status_bad_request()).done();
        }

        cp::query get_page("SELECT * FROM \"posts\" WHERE \"post_id\"=($1);");

        auto tx = cp::tx(*pool_ptr, get_page);

        pqxx::result result = get_page(postid);
        if (result[0]["is_secret"].as<bool>() == true, result[0]["is_published"].as<bool>() == false) {
            logger_ptr->info( [endpoint]{return fmt::format("page request from {} is secret", endpoint);});
            return req->create_response(restinio::status_non_authoritative_information()).done();
        }
        std::cout<<"result: "<<result[0]["post_path"].as<std::string>()<<std::endl;

        if(result.empty()) {
            return req->create_response(restinio::status_bad_gateway()).done();
        }

        return req->create_response().set_body(cp::serialize(result)).done();
        
    });
}

void add_page_by_content(std::unique_ptr<restinio::router::express_router_t<>>& router, std::shared_ptr<cp::connection_pool> pool_ptr, std::shared_ptr<restinio::shared_ostream_logger_t> logger_ptr) {
    router.get()->http_post("/add_page_content", [pool_ptr, logger_ptr](auto req, auto) {
        std::string endpoint = req->remote_endpoint().address().to_string();

        logger_ptr->info( [endpoint]{return fmt::format("page request from {}", endpoint);});

        rapidjson::Document new_body;
        new_body.Parse(req->body().c_str());

        std::unordered_map<std::string, std::any> new_map = parse_page_content(new_body);

        if (!new_map.empty()) {
            std::string path = std::any_cast<std::string>(new_map["post_path"]); 
            logger_ptr->info( [endpoint, path]{return fmt::format("path from {} is {}", endpoint, path);});
            if(path[0] == '/' || path.find("..") != std::string::npos) {
                logger_ptr->info( [endpoint]{return fmt::format("bad path from {}", endpoint);});
                return req->create_response(restinio::status_forbidden()).done();
            }
            cp::query add_page("INSERT INTO \"posts\" (\"post_path\", \"is_secret\", \"is_published\", \"likes\", \"title\", \"author\", \"text\") VALUES ($1, $2, $3, $4, $5, $6, $7) RETURNING \"post_id\";");
            auto tx = cp::tx(*pool_ptr, add_page);
            pqxx::result res;
            try {
                res = add_page(path, 
                        std::any_cast<bool>(new_map["is_secret"]), 
                        std::any_cast<bool>(new_map["is_published"]), 
                        std::any_cast<int>(new_map["likes"]), 
                        std::any_cast<std::string>(new_map["title"]), 
                        std::any_cast<std::string>(new_map["author"]), 
                        std::any_cast<std::string>(new_map["text"]));
            } catch (pqxx::sql_error& e) {
                logger_ptr->error( [endpoint]{return fmt::format("sql error from {}", endpoint);});
                return req->create_response(restinio::status_non_authoritative_information()).done();
            } catch (std::bad_any_cast& e) {
                logger_ptr->error( [endpoint]{return fmt::format("bad request from {}", endpoint);});
                return req->create_response(restinio::status_non_authoritative_information()).done();
            }
            int post_id = res[0]["post_id"].as<int>();
            
            logger_ptr->info( [post_id]{return fmt::format("page added with id {}", post_id);});
            try {
                std::filesystem::copy_file(std::filesystem::current_path() / std::filesystem::path("letovo-wiki/blank_page.html"), path, std::filesystem::copy_options::overwrite_existing);
                add_id_to_page(path, post_id, logger_ptr);
            } catch (std::filesystem::filesystem_error& e) {
                logger_ptr->error( [endpoint, path, e]{return fmt::format("path error {} with path {}, {}", endpoint, path, e.what());});
                return req->create_response(restinio::status_bad_request()).done();
            }
            return req->create_response().done();
        }
        else {
            logger_ptr->info( [endpoint]{return fmt::format("bad request from {}, not enough args", endpoint);});
            return req->create_response(restinio::status_non_authoritative_information()).done();
        }
    });
}

void add_page_by_page(std::unique_ptr<restinio::router::express_router_t<>>& router, std::shared_ptr<cp::connection_pool> pool_ptr, std::shared_ptr<restinio::shared_ostream_logger_t> logger_ptr) {
    router.get()->http_post("/add_page", [pool_ptr, logger_ptr](auto req, auto) {
        std::string endpoint = req->remote_endpoint().address().to_string();

        logger_ptr->info( [endpoint]{return fmt::format("page request from {}", endpoint);});

        rapidjson::Document new_body;
        new_body.Parse(req->body().c_str());

        if (new_body.HasMember("post_path")) {
            std::string path = new_body["post_path"].GetString();
            if(path[0] == '/' || path.find("..") != std::string::npos) {
                logger_ptr->info( [endpoint]{return fmt::format("bad path from {}", endpoint);});
                return req->create_response(restinio::status_forbidden()).done();
            }
            cp::query add_page("INSERT INTO \"posts\" (\"post_path\") VALUES ($1) RETURNING \"post_id\";");

            auto tx = cp::tx(*pool_ptr, add_page);

            pqxx::result res = add_page(path);
            int post_id = res[0]["post_id"].as<int>();
            logger_ptr->info( [endpoint, path]{return fmt::format("page added from {} to {}", endpoint, path);});
            create_file(path, new_body["text"].GetString(), logger_ptr);
            return req->create_response(restinio::status_accepted()).done();
        }
        else return req->create_response(restinio::status_non_authoritative_information()).done();
    });
}