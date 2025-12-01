#include <filesystem>
#include <iostream>
#include <sstream>
#include <string>
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

std::vector<std::string> splitString(const std::string &s, char delimiter) {
  std::vector<std::string> tokens;
  std::string token;
  size_t start = 0;
  size_t end = s.find(delimiter);

  while (end != std::string::npos) {
    token = s.substr(start, end - start);
    tokens.push_back(token);
    start = end + 1;
    end = s.find(delimiter, start);
  }

  tokens.push_back(s.substr(start));
  return tokens;
}

void exitCommand() { exit(0); }

void echoCommand(vector<string> args) {
  string echoOutput;
  for (auto arg : args) {
    echoOutput += arg;
    echoOutput += " ";
  }
  cout << echoOutput << endl;
}

bool is_builtin(string arg) {
  const vector<string> builtins{"echo", "type", "exit"};
  auto it = find(builtins.begin(), builtins.end(), arg);
  if (it != builtins.end()) {
    cout << arg << " is a shell builtin" << endl;
    return true;
  }

  const char *env_p = std::getenv("PATH");
  auto pathDirectories = splitString(env_p, ':');
  for (auto dir : pathDirectories) {
    auto fullPath = std::filesystem::path(dir) / arg;
    // Check if file exists, is a regular file, and has execute permissions
    if (std::filesystem::exists(fullPath) &&
        std::filesystem::is_regular_file(fullPath)) {
      auto perms = std::filesystem::status(fullPath).permissions();
      if ((perms & std::filesystem::perms::owner_exec) !=
              std::filesystem::perms::none ||
          (perms & std::filesystem::perms::group_exec) !=
              std::filesystem::perms::none ||
          (perms & std::filesystem::perms::others_exec) !=
              std::filesystem::perms::none) {
        cout << arg << " is " << fullPath.string() << endl;
        return true;
      }
    }
  }
  cout << arg << ": not found" << endl;
  return false;
}

// bool is_builtin(string arg) {
//   const vector<string> builtins{"echo", "type", "exit"};
//
//   // if present in my custom commands, might as well return
//   auto it = find(builtins.begin(), builtins.end(), arg);
//   if (it != builtins.end()) {
//     cout << arg << " is a shell builtin";
//     return true;
//   }
//
//   // else check in the PATH
//   const char *env_p = std::getenv("PATH");
//   auto pathDirectories = splitString(env_p, ':');
//   for (auto dir : pathDirectories) {
//     // cout << dir << endl;
//     if (std::filesystem::exists(std::filesystem::path(dir) / arg)) {
//       cout << arg << " is " << string(std::filesystem::path(dir) / arg) <<
//       endl; return true;
//     }
//   }
//
//   cout << arg << ": not found" << endl;
//   return false;
// }

void typeCommand(string arg) {
  is_builtin(arg);
  // if (is_builtin(arg)) {
  //   cout << arg << " is a shell builtin" << endl;
  // } else {
  //   cout << arg << ": not found" << endl;
  // }
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

  // if (std::filesystem::exists(env_p) && std::filesystem::is_directory(env_p))
  // {
  //   for (const auto &entry : std::filesystem::directory_iterator(env_p)) {
  //     std::cout << entry.path().filename() << std::endl;
  //   }
  // } else {
  //   std::cerr << "Directory not found or is not a directory." << std::endl;
  // }

  repl();
}
