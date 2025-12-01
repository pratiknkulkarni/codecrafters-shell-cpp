#include <iostream>
#include <string>

using namespace std;

void exitCommand() {
    exit(0);
}

void repl() {
    string command;
    while (true) {
        cout << "$ ";
        cin >> command;

        if (command == "exit") {
            exitCommand();
        }

        cout << command << ": command not found" << endl;
    }
}

int main() {
    cout << std::unitbuf;
    cerr << std::unitbuf;

    repl();
}
