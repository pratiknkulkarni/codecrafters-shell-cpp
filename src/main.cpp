#include <cstdlib>
#include <iostream>
#include "shell/shell.h"

int main() {
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  shell::Shell shell;
  shell.run();

  return EXIT_SUCCESS;
}
