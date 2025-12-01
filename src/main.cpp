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
