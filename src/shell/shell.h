//
// Created by pnkulkarni on 11/12/25.
//

#ifndef SHELL_STARTER_CPP_SHELL_H
#define SHELL_STARTER_CPP_SHELL_H
#include <memory>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>
#include"../commands/Command.h"


namespace shell {
    class Shell {
    public:
        Shell();

        ~Shell();

        void run();

    private:
        std::vector<std::string> builtin_names_;
        std::unordered_map<std::string, std::unique_ptr<commands::Command> > commands_;
        std::string histfile_;
        int last_appended_index_ = 0;

        void initialize_builtins();

        void load_history_file();

        void save_history_file();

        std::optional<std::string> read_input() const;

        void execute_command(const std::string &input);

        void execute_external_command(const std::string &command,
                                      const std::vector<std::string> &args);
    };
}


#endif //SHELL_STARTER_CPP_SHELL_H
