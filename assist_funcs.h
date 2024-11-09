#pragma once

#include <unordered_map>
#include <any>
#include <filesystem>
#include <fstream>
#include <restinio/all.hpp>
#include <fmt/format.h>
#include <rapidjson/document.h>

namespace assist{
    std::unordered_map<std::string, std::any> parse_page_content(rapidjson::Document& new_body);

    void add_id_to_page(const std::string& filePath, int number, std::shared_ptr<restinio::shared_ostream_logger_t> logger_ptr);

    void create_file(const std::string &filePath, const std::string &content, std::shared_ptr<restinio::shared_ostream_logger_t> logger_ptr);
}