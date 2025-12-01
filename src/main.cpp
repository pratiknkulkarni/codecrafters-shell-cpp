#include <iostream>
#include <string>

using namespace std;

int main() {
  string command;

  cout << std::unitbuf;
  cerr << std::unitbuf;

  cout << "$ ";
  cin >> command;
  cout << command << ": command not found" << endl;
}
