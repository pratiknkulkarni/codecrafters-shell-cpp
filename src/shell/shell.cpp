//
// Created by pnkulkarni on 11/12/25.
//

#include "shell.h"

#include <iostream>
#include <readline/history.h>
#include <readline/readline.h>
#include <sys/wait.h>
#include <cstring>


#include "../commands/builtin_commands.h"
#include "../utils/path_utils.h"
#include "../utils/string_utils.h"

namespace shell {
    Shell::Shell() {
        load_history_file();
        initialize_builtins();
    }

    Shell::~Shell() {
        save_history_file();
    }

    void Shell::load_history_file() {
        const char *hf = std::getenv("HISTFILE");
        if (!hf) {
            return;
        }
        histfile_ = hf;
        read_history(histfile_.c_str());
    }

    void Shell::save_history_file() {
        if (histfile_.empty()) {
            return;
        }
        if (write_history(histfile_.c_str()) != 0) {
            std::cerr << "error: could not write history to " << histfile_ << '\n';
        }
    }

    void Shell::initialize_builtins() {
        builtin_names_ = {"exit", "echo", "pwd", "cd", "type", "history"};

        commands_["exit"] = std::make_unique<commands::ExitCommand>(histfile_);
        commands_["echo"] = std::make_unique<commands::EchoCommand>();
        commands_["pwd"] = std::make_unique<commands::PwdCommand>();
        commands_["cd"] = std::make_unique<commands::CdCommand>();
        commands_["type"] = std::make_unique<commands::TypeCommand>(builtin_names_);
        commands_["history"] = std::make_unique<commands::HistoryCommand>(last_appended_index_);
    }

    std::optional<std::string> Shell::read_input() const {
        char *line_cstr = readline("$ ");
        if (!line_cstr) {
            return std::nullopt;
        }

        std::string line(line_cstr);

        free(line_cstr);
        return line;
    }

    void Shell::execute_command(const std::string &input) {
        const auto tokens = utils::tokenize(input);
        if (tokens.empty()) {
            return;
        }

        const auto &command_name = tokens[0];
        const std::vector<std::string> args(tokens.begin() + 1, tokens.end());

        if (auto it = commands_.find(command_name); it != commands_.end()) {
            it->second->execute(args);
        } else {
            execute_external_command(command_name, args);
        }
    }

    void Shell::execute_external_command(const std::string &command,
                                         const std::vector<std::string> &args) {
        const auto path = utils::find_in_path(command);
        if (!path) {
            std::cout << command << ": command not found\n";
            return;
        }

        // Build argv: argv[0] is the command as the user typed (unquoted token),
        // following entries are the args, terminated by nullptr.
        std::vector<std::string> argv_strings;
        argv_strings.push_back(command); // argv[0]
        for (const auto &a: args) argv_strings.push_back(a);

        std::vector<char *> argv;
        argv.reserve(argv_strings.size() + 1);
        for (auto &s: argv_strings) {
            argv.push_back(const_cast<char *>(s.c_str()));
        }
        argv.push_back(nullptr);

        pid_t pid = fork();
        if (pid < 0) {
            std::cerr << "error: fork failed: " << std::strerror(errno) << '\n';
            return;
        }

        if (pid == 0) {
            execv(path->c_str(), argv.data());
            std::cerr << "error: exec failed: " << std::strerror(errno) << '\n';
            _exit(127);
        } else {
            int status = 0;
            while (true) {
                pid_t w = waitpid(pid, &status, 0);
                if (w == -1) {
                    if (errno == EINTR) continue;
                    std::cerr << "error: waitpid failed: " << std::strerror(errno) << '\n';
                    break;
                }
                break;
            }
        }
    }

    void Shell::run() {
        while (true) {
            const auto input = read_input();
            if (!input) {
                break;
            }

            if (input->empty()) {
                continue;
            }

            add_history(input->c_str());
            execute_command(*input);
        }
    }
}
