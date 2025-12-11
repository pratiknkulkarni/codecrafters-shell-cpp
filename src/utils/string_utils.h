//
// Created by pnkulkarni on 11/12/25.
//

#ifndef SHELL_STARTER_CPP_STRING_UTILS_H
#define SHELL_STARTER_CPP_STRING_UTILS_H
#include<vector>

#include <sstream>

namespace utils {
    std::vector<std::string> tokenize(std::string_view line);
    std::vector<std::string> split(std::string_view str, char delimiter);
}

class string_utils {
};


#endif //SHELL_STARTER_CPP_STRING_UTILS_H
