#pragma once
#include <restinio/all.hpp>
#include <pqxx/pqxx>
#include <fmt/format.h>
#include <rapidjson/document.h>
#include "../pqxx_cp.h"
#include <unordered_map>
#include <filesystem>
#include <any>
#include "assist_funcs.h"

void get_page_content(std::unique_ptr<restinio::router::express_router_t<>>& router, std::shared_ptr<cp::connection_pool> pool_ptr, std::shared_ptr<restinio::shared_ostream_logger_t> logger_ptr);

void add_page_by_content(std::unique_ptr<restinio::router::express_router_t<>>& router, std::shared_ptr<cp::connection_pool> pool_ptr, std::shared_ptr<restinio::shared_ostream_logger_t> logger_ptr);

void add_page_by_page(std::unique_ptr<restinio::router::express_router_t<>>& router, std::shared_ptr<cp::connection_pool> pool_ptr, std::shared_ptr<restinio::shared_ostream_logger_t> logger_ptr);

std::unordered_map<std::string, std::any> parse_page_content(rapidjson::Document& new_body);

void add_id_to_page(const std::string& filePath, int number, std::shared_ptr<restinio::shared_ostream_logger_t> logger_ptr);