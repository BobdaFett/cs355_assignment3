#include "sub.h"
#include <iostream>

int main() {
  std::cout << "Hello world!" << std::endl;

  Sub mainFunc("main_", nullptr, 0, 0);
  Sub temp("helloFunction", &mainFunc, 1, 2);
  mainFunc.call_();
  Sub::printStacks();

  temp.call_();
  Sub::printStacks();

  temp.return_();
  Sub::printStacks();

  return 0;
}
