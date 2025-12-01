#include <iostream>
#include <ranges>
#include <string>
#include <sstream>
#include <vector>

using namespace std;


std::vector<std::string> tokenize(const string &line) {
    vector<string> tokens;
    stringstream ss(line);
    string token;

    while (ss >> token) {
        tokens.push_back(token);
    }

    return tokens;
}

void exitCommand() {
    exit(0);
}

void echoCommand(vector<string> args) {
    string echoOutput;
    for (auto arg: args) {
        echoOutput += arg;
        echoOutput += " ";
    }
    cout << echoOutput << endl;
}

void typeCommand(string arg) {
    vector<string> builtins{"echo", "type", "exit"};
    auto it = find(builtins.begin(), builtins.end(), arg);
    if (it != builtins.end()) {
        cout << arg << " is a shell builtin" << endl;
    } else {
        cout << arg << ": not found" << endl;
    }
}

void repl() {
    string userInput;

    while (true) {
        cout << "$ ";
        if (!getline(cin, userInput)) {
            break;
        }
        auto tokens = tokenize(userInput);
        if (tokens.empty()) {
            continue;
        }

        const string &command = tokens[0];
        vector<string> arguments(tokens.begin() + 1, tokens.end());

        if (command == "exit") {
            exitCommand();
        } else if (command == "echo") {
            echoCommand(arguments);
        } else if (command == "type") {
            if (arguments.size() != 1) {
                exit(1);
            } else {
                typeCommand(arguments[0]);
            }
        } else {
            cout << userInput << ": command not found" << endl;
        }
    }
}

int main() {
    cout << std::unitbuf;
    cerr << std::unitbuf;

    repl();
}
