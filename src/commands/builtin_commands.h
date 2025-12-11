//
// Created by pnkulkarni on 11/12/25.
//

#pragma once


#ifndef SHELL_STARTER_CPP_BUILTIN_COMMANDS_H
#define SHELL_STARTER_CPP_BUILTIN_COMMANDS_H
#include "Command.h"

namespace commands {
    class ExitCommand : public Command {
    public:
        explicit ExitCommand(std::string &histfile);

        void execute(const std::vector<std::string> &args) override;

    private:
        std::string &histfile_;

        void save_history_file();
    };


    class EchoCommand : public Command {
    public:
        void execute(const std::vector<std::string>& args) override;
    };

    class PwdCommand : public Command {
    public:
        void execute(const std::vector<std::string>& args) override;
    };

    class TypeCommand : public Command {
    public:
        explicit TypeCommand(const std::vector<std::string>& builtins);
        void execute(const std::vector<std::string>& args) override;

    private:
        const std::vector<std::string>& builtins_;
    };

    class CdCommand : public Command {
    public:
        void execute(const std::vector<std::string>& args) override;

    private:
        void change_to_home();
        void change_directory(const std::string& path);
    };

    class HistoryCommand : public Command {
    public:
        explicit HistoryCommand(int& last_appended_index);
        void execute(const std::vector<std::string>& args) override;

    private:
        int& last_appended_index_;

        void print_all_history() const;
        void print_limited_history(const std::string& limit_str) const;
        void handle_history_file_operation(const std::string& flag,
                                           const std::string& filepath);
        void read_history_from_file(const std::string& filepath);
        void append_new_history(const std::string& filepath);
        void write_history_to_file(const std::string& filepath);
    };

}

#endif //SHELL_STARTER_CPP_BUILTIN_COMMANDS_H
