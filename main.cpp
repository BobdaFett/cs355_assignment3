#include <iostream>
#include "sub.h"

int main() {
  std::cout << "Hello world!" << std::endl;

  // These subprograms follow the same structure as the subprograms defined in the class slides
  // and in the textbook.
  Sub mainFunc("main_", nullptr, 0, 0);
  Sub sub1("sub1", &mainFunc, 1, 2);
  Sub sub2("sub2", &mainFunc, 0, 0);
  Sub sub3("sub3", &sub2, 0, 0);

  mainFunc.call_();
  Sub::printStacks();

  sub2.call_();
  Sub::printStacks();

  sub3.call_();
  Sub::printStacks();

  sub1.call_();
  Sub::printStacks();

  sub1.return_();
  Sub::printStacks();

  sub3.return_();
  Sub::printStacks();

  sub2.return_();
  Sub::printStacks();

  mainFunc.return_();
  Sub::printStacks();

  return 0;
}
