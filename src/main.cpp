#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <readline/history.h>
#include <readline/readline.h>
#include <sstream>
#include <string>
#include <unistd.h>
#include <vector>

using namespace std;

std::vector<std::string> tokenize(const string &line) {
  vector<string> tokens;
  stringstream ss(line);
  string token;

  while (ss >> token) {
    // cout << "pushing token = " << token << endl;
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

void pwd_command() { cout << filesystem::current_path().string() << endl; }

void type_command(const std::string &arg) {
  // start with my custom builtins
  const vector<string> builtins{"echo", "type", "exit", "pwd", "history"};
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

void cd_command(string arg) {
  if (arg == "~" || arg == " ") {
    auto home = getenv("HOME");

    filesystem::current_path(home);
    return;
  }

  if (filesystem::exists(arg) && filesystem::is_directory(arg)) {
    filesystem::current_path(arg);
  } else {
    cout << "cd: " << arg << ": No such file or directory" << endl;
  }
}

void history_command() {
  HIST_ENTRY **the_list = history_list();
  if (!the_list) {
    return;
  };
  for (int i = 0; i < history_length; ++i) {
    std::cout << "  " << (history_base + i) << " " << the_list[i]->line
              << std::endl;
  }
}

void history_command_limit(vector<string> history_stack, int limit) {
  HIST_ENTRY **the_list = history_list();
  if (!the_list) {
    return;
  }

  int total = history_length;

  if (limit >= total) {
    history_command();
    return;
  }

  int start = total - limit;

  for (int i = start; i < total; ++i) {
    std::cout << "  " << (history_base + i) << " " << the_list[i]->line
              << std::endl;
  }
}

void custom_command(const string &command, vector<string> arguments) {
  string path_with_args = command + " ";

  for (string args : arguments) {
    path_with_args += args;
    path_with_args += " ";
  }

  system(path_with_args.c_str());
}

void repl() {
  vector<string> history;

  while (true) {
    char *line_cstr = readline("$ ");

    if (!line_cstr) {
      break;
    }

    string userInput(line_cstr);
    free(line_cstr);

    if (userInput.empty()) {
      continue;
    }

    add_history(userInput.c_str());
    auto tokens = tokenize(userInput);
    if (tokens.empty()) {
      continue;
    }

    const string &command = tokens[0];
    vector<string> arguments(tokens.begin() + 1, tokens.end());

    history.push_back(userInput);
    if (command == "exit") {
      exit_command();
    } else if (command == "echo") {
      echo_command(arguments);
    } else if (command == "pwd") {
      pwd_command();
    } else if (command == "cd") {
      if (arguments.size() != 1) {
        exit(1);
      }
      cd_command(arguments[0]);
    } else if (command == "type") {
      if (arguments.size() != 1) {
        exit(1);
      } else {
        type_command(arguments[0]);
      }
    } else if (command == "history") {

      if (arguments.size() == 1) {
        int limit = stoi(arguments[0]);
        history_command_limit(history, limit);
      } else if (arguments.size() == 2) {
        string flag = arguments[0];
        string filePath = arguments[1];

        ifstream inputFile(filePath);
        if (!inputFile.is_open()) {
          cerr << "error opening file " << endl;
        }

        string line;
        while (getline(inputFile, line)) {
          add_history(line.c_str());
        }

      } else {
        history_command();
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
