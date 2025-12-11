//
// Created by pnkulkarni on 11/12/25.
//

#include "string_utils.h"
#include<vector>
#include <sstream>


namespace utils {
    std::vector<std::string> tokenize(std::string_view line) {
        std::vector<std::string> tokens;
        std::istringstream stream{std::string(line)};
        std::string token;

        while (stream >> token) {
            tokens.push_back(std::move(token));
        }

        return tokens;
    }

    std::vector<std::string> split(std::string_view str, char delimiter) {
        std::vector<std::string> tokens;
        size_t start = 0;
        size_t end = str.find(delimiter);

        while (end != std::string_view::npos) {
            tokens.emplace_back(str.substr(start, end - start));
            start = end + 1;
            end = str.find(delimiter, start);
        }

        tokens.emplace_back(str.substr(start));
        return tokens;
    }
}
