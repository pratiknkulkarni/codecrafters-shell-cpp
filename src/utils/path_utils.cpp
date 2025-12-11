//
// Created by pnkulkarni on 11/12/25.
//

#include "path_utils.h"
#include <cstdlib>
#include "string_utils.h"

namespace fs = std::filesystem;

namespace utils {
    bool is_executable(const fs::path &path) noexcept {
        try {
            if (!fs::exists(path) || !fs::is_regular_file(path)) {
                return false;
            }

            const auto perms = fs::status(path).permissions();
            return (perms & fs::perms::owner_exec) != fs::perms::none ||
                   (perms & fs::perms::group_exec) != fs::perms::none ||
                   (perms & fs::perms::others_exec) != fs::perms::none;
        } catch (const fs::filesystem_error &) {
            return false;
        }
    }

    std::optional<std::string> find_in_path(std::string_view executable_name) {
        const char *env_path = std::getenv("PATH");
        if (!env_path) {
            return std::nullopt;
        }

        const auto directories = split(env_path, ':');
        for (const auto &dir: directories) {
            const auto full_path = fs::path(dir) / executable_name;
            if (is_executable(full_path)) {
                return full_path.string();
            }
        }

        return std::nullopt;
    }
}
