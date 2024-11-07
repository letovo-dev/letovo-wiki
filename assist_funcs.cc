#include "assist_funcs.h"
#include <iostream>

std::unordered_map<std::string, std::any> parse_page_content(rapidjson::Document& new_body) {
    std::unordered_map<std::string, std::any> new_map;
    if(new_body.HasMember("author")) {
        std::string author = new_body["author"].GetString();
        new_map["author"] = std::any(author);
    } else {
        new_map["author"] = std::any("");
    }
    if(new_body.HasMember("is_secret") && new_body["is_secret"].GetBool()) {
        new_map["is_secret"] = std::any(true);
    } else {
        new_map["is_secret"] = std::any(false);
    }
    if(new_body.HasMember("is_published") && new_body["is_published"].GetBool()) {
        new_map["is_published"] = std::any(true);
    } else {
        new_map["is_published"] = std::any(false);
    }
    if(new_body.HasMember("post_path")) {
        std::string post_path = new_body["post_path"].GetString();
        new_map["post_path"] = std::any(post_path);
    } else {
        return {};
    }
    if(new_body.HasMember("likes")) {
        int likes = new_body["likes"].GetInt();
        new_map["likes"] = std::any(likes);
    } else {
        new_map["likes"] = std::any(0);
    }
    if(new_body.HasMember("text")) {
        std::string text = new_body["text"].GetString();
        new_map["text"] = std::any(text);
    } else {
        return {};
    }
    if(new_body.HasMember("title")) {
        std::string title = new_body["title"].GetString();
        new_map["title"] = std::any(title);
    } else {
        new_map["title"] = std::any("");
    }
    return new_map;
}

void add_id_to_page(const std::string& filePath, int number, std::shared_ptr<restinio::shared_ostream_logger_t> logger_ptr) {
    if (!std::filesystem::exists(filePath)) {
        logger_ptr->error( [filePath]{return fmt::format("File does not exist: {}", filePath);});
        return;
    }
    std::ifstream inFile(filePath);
    if (!inFile) {
        logger_ptr->error( [filePath]{return fmt::format("Failed to open file for reading: {}", filePath);});
        return;
    }
    std::string firstLine;
    std::getline(inFile, firstLine); 

    std::string remainingContent;
    if (inFile) {
        remainingContent.assign((std::istreambuf_iterator<char>(inFile)),
                                 std::istreambuf_iterator<char>());
    }
    inFile.close();

    firstLine += " " + std::to_string(number);

    std::ofstream outFile(filePath);
    if (!outFile) {
        logger_ptr->error( [filePath]{return fmt::format("Failed to open file for writing: {}", filePath);});
        return;
    }

    outFile << firstLine << '\n' << remainingContent;
    outFile.close();

    logger_ptr->info( [filePath]{return fmt::format("Added id to file: {}", filePath);});
}

void create_file(const std::string &filePath, const std::string &content, std::shared_ptr<restinio::shared_ostream_logger_t> logger_ptr) {
    std::ofstream outFile(filePath);
    if (!outFile) {
        logger_ptr->error( [filePath]{return fmt::format("Failed to open file for writing: {}", filePath);});
        return;
    }

    outFile << content;
    outFile.close();

    logger_ptr->info( [filePath]{return fmt::format("Created file: {}", filePath);});
}