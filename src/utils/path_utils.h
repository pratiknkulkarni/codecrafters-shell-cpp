//
// Created by pnkulkarni on 11/12/25.
//

#ifndef SHELL_STARTER_CPP_PATH_UTILS_H
#define SHELL_STARTER_CPP_PATH_UTILS_H

#pragma once

#include <filesystem>
#include <optional>
#include <string>
#include <string_view>

namespace utils {
    bool is_executable(const std::filesystem::path &path) noexcept;

    std::optional<std::string> find_in_path(std::string_view executable_name);
}


class path_utils {
};


#endif //SHELL_STARTER_CPP_PATH_UTILS_H
