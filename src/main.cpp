#include <cstdlib>
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

void exit_command() { exit(0); }

void echo_command(vector<string> args) {
  string echoOutput;
  for (auto arg : args) {
    echoOutput += arg;
    echoOutput += " ";
  }
  cout << echoOutput << endl;
}

bool is_executable(const std::filesystem::path &path) {
  try {
    if (!std::filesystem::exists(path) ||
        !std::filesystem::is_regular_file(path)) {
      return false;
    }
    auto perms = std::filesystem::status(path).permissions();
    return (perms & std::filesystem::perms::owner_exec) !=
               std::filesystem::perms::none ||
           (perms & std::filesystem::perms::group_exec) !=
               std::filesystem::perms::none ||
           (perms & std::filesystem::perms::others_exec) !=
               std::filesystem::perms::none;
  } catch (const std::filesystem::filesystem_error &) {
    return false;
  }
}

std::string find_executable_in_path(const std::string &name) {
  const char *env_path = std::getenv("PATH");
  if (!env_path) {
    return "";
  }

  auto pathDirectories = splitString(env_path, ':');
  for (const auto &dir : pathDirectories) {
    auto fullPath = std::filesystem::path(dir) / name;
    if (is_executable(fullPath)) {
      return fullPath.string();
    }
  }
  return "";
}

void type_command(const std::string &arg) {
  // start with my custom builtins
  const vector<string> builtins{"echo", "type", "exit"};
  if (find(builtins.begin(), builtins.end(), arg) != builtins.end()) {
    cout << arg << " is a shell builtin" << endl;
    return;
  }

  // else go to the pathos
  std::string path = find_executable_in_path(arg);
  if (!path.empty()) {
    cout << arg << " is " << path << endl;
  } else {
    cout << arg << ": not found" << endl;
  }
}

void custom_command(const string &command, vector<string> arguments) {
  auto path = find_executable_in_path(command);
  string path_with_args = command + " ";

  for (string args : arguments) {
    path_with_args += args;
    path_with_args += " ";
  }

  system(path_with_args.c_str());
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
      exit_command();
    } else if (command == "echo") {
      echo_command(arguments);
    } else if (command == "type") {
      if (arguments.size() != 1) {
        exit(1);
      } else {
        type_command(arguments[0]);
      }
    } else {
      auto path = find_executable_in_path(command);
      if (path == "") {
        cout << userInput << ": command not found" << endl;
      } else {
        custom_command(command, arguments);
      }
    }
  }
}

int main() {
  cout << std::unitbuf;
  cerr << std::unitbuf;

  repl();
}
