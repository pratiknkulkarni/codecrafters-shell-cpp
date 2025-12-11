//
// Created by pnkulkarni on 11/12/25.
//

#ifndef SHELL_STARTER_CPP_COMMAND_H
#define SHELL_STARTER_CPP_COMMAND_H
#include <string>
#include <vector>

namespace commands {
    class Command {
    public:
        virtual ~Command() = default;

        virtual void execute(const std::vector<std::string> &args) = 0;
    };
}

#endif //SHELL_STARTER_CPP_COMMAND_H
