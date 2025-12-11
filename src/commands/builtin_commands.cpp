//
// Created by pnkulkarni on 11/12/25.
//

#include "builtin_commands.h"
#include <readline/history.h>
#include <filesystem>
#include <iostream>

#include "Command.h"

#include <algorithm>
#include <cstdlib>
#include <fstream>

#include "../utils/path_utils.h"

namespace fs = std::filesystem;

namespace commands {
    ExitCommand::ExitCommand(std::string &histfile) : histfile_(histfile) {
    }

    void ExitCommand::save_history_file() {
        if (histfile_.empty()) {
            return;
        }
        if (write_history(histfile_.c_str()) != 0) {
            std::cerr << "error: could not write history to " << histfile_ << '\n';
        }
    }

    void ExitCommand::execute(const std::vector<std::string> &) {
        save_history_file();
        std::exit(EXIT_SUCCESS);
    }

    // EchoCommand
    void EchoCommand::execute(const std::vector<std::string> &args) {
        if (!args.empty()) {
            std::cout << args[0];
            for (size_t i = 1; i < args.size(); ++i) {
                std::cout << ' ' << args[i];
            }
        }
        std::cout << '\n';
    }

    // PwdCommand
    void PwdCommand::execute(const std::vector<std::string> &) {
        std::cout << fs::current_path().string() << '\n';
    }

    // TypeCommand
    TypeCommand::TypeCommand(const std::vector<std::string> &builtins)
        : builtins_(builtins) {
    }

    void TypeCommand::execute(const std::vector<std::string> &args) {
        if (args.empty()) {
            std::cerr << "type: missing argument\n";
            return;
        }

        const auto &name = args[0];

        if (std::find(builtins_.begin(), builtins_.end(), name) != builtins_.end()) {
            std::cout << name << " is a shell builtin\n";
            return;
        }

        if (const auto path = utils::find_in_path(name)) {
            std::cout << name << " is " << *path << '\n';
        } else {
            std::cout << name << ": not found\n";
        }
    }

    // CdCommand
    void CdCommand::execute(const std::vector<std::string> &args) {
        if (args.empty()) {
            change_to_home();
            return;
        }

        const auto &target = args[0];
        if (target == "~") {
            change_to_home();
        } else {
            change_directory(target);
        }
    }

    void CdCommand::change_to_home() {
        if (const char *home = std::getenv("HOME")) {
            std::error_code ec;
            fs::current_path(home, ec);
            if (ec) {
                std::cerr << "cd: " << ec.message() << '\n';
            }
        } else {
            std::cerr << "cd: HOME not set\n";
        }
    }

    void CdCommand::change_directory(const std::string &path) {
        std::error_code ec;

        if (!fs::exists(path, ec)) {
            std::cerr << "cd: " << path << ": No such file or directory\n";
            return;
        }

        if (!fs::is_directory(path, ec)) {
            std::cerr << "cd: " << path << ": Not a directory\n";
            return;
        }

        fs::current_path(path, ec);
        if (ec) {
            std::cerr << "cd: " << ec.message() << '\n';
        }
    }

    // HistoryCommand
    HistoryCommand::HistoryCommand(int &last_appended_index)
        : last_appended_index_(last_appended_index) {
    }

    void HistoryCommand::execute(const std::vector<std::string> &args) {
        if (args.empty()) {
            print_all_history();
        } else if (args.size() == 1) {
            print_limited_history(args[0]);
        } else if (args.size() == 2) {
            handle_history_file_operation(args[0], args[1]);
        } else {
            std::cerr << "history: too many arguments\n";
        }
    }

    void HistoryCommand::print_all_history() const {
        HIST_ENTRY **entries = history_list();
        if (!entries) {
            return;
        }

        for (int i = 0; i < history_length; ++i) {
            std::cout << "  " << (history_base + i) << "  "
                    << entries[i]->line << '\n';
        }
    }

    void HistoryCommand::print_limited_history(const std::string &limit_str) const {
        try {
            const int limit = std::stoi(limit_str);
            if (limit <= 0) {
                std::cerr << "history: invalid limit\n";
                return;
            }

            HIST_ENTRY **entries = history_list();
            if (!entries) {
                return;
            }

            const int total = history_length;
            const int start = std::max(0, total - limit);

            for (int i = start; i < total; ++i) {
                std::cout << "  " << (history_base + i) << "  "
                        << entries[i]->line << '\n';
            }
        } catch (const std::exception &) {
            std::cerr << "history: invalid limit\n";
        }
    }

    void HistoryCommand::handle_history_file_operation(const std::string &flag,
                                                       const std::string &filepath) {
        if (flag == "-r") {
            read_history_from_file(filepath);
        } else if (flag == "-a") {
            append_new_history(filepath);
        } else if (flag == "-w") {
            write_history_to_file(filepath);
        } else {
            // Treat as -w flag if no flag specified (backward compatibility)
            write_history_to_file(filepath);
        }
    }

    void HistoryCommand::read_history_from_file(const std::string &filepath) {
        std::ifstream file(filepath);
        if (!file) {
            std::cerr << "history: error opening file: " << filepath << '\n';
            return;
        }

        std::string line;
        while (std::getline(file, line)) {
            add_history(line.c_str());
        }
    }

    void HistoryCommand::append_new_history(const std::string &filepath) {
        const int current_length = history_length;
        const int entries_to_append = current_length - last_appended_index_;

        if (entries_to_append > 0) {
            if (append_history(entries_to_append, filepath.c_str()) != 0) {
                std::cerr << "history: error appending to file\n";
            } else {
                last_appended_index_ = current_length;
            }
        }
    }

    void HistoryCommand::write_history_to_file(const std::string &filepath) {
        if (write_history(filepath.c_str()) != 0) {
            std::cerr << "history: error writing to file\n";
        }
    }
}
