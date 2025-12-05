#include <iostream>

#include "nameRegistry.h"
#include "sub.h"

void test_equal_depths();
void test_caller_lower_depth();
void test_caller_higher_depth();
void test_missing_parent();
void test_return_depth();
void test_return_depth_error();

int main() {
  std::cout << "Hello world!" << std::endl;

  // These subprograms follow the same structure as the subprograms defined in the class slides
  // and in the textbook.

  test_return_depth();
  test_return_depth_error();

  return 0;
}

void test_equal_depths() {
  std::cout << "Testing equal depths." << std::endl;
  std::cout << "Calls at equal depths should work, resulting in no errors.\n" << std::endl;

  Sub subMain("main_", nullptr, 0, 0);
  Sub subSub1("sub1", &subMain, 0, 0);
  Sub subSub2("sub2", &subMain, 0, 0);

  subMain.call_();
  subSub1.call_();
  subSub2.call_();
  Sub::printStacks();

  Sub::clear();
  NameRegistry::clear();
}

void test_caller_lower_depth() {
  std::cout << "Testing calling a subprogram that is at a lower depth (less nesting, calling a child)." << std::endl;
  std::cout << "Calls at lower depths should work, resulting in no errors.\n" << std::endl;

  Sub mainFunc("main_", nullptr, 0, 0);
  Sub sub1("sub2", &mainFunc, 0, 0);
  Sub sub2("sub3", &sub2, 0, 0);

  mainFunc.call_();
  sub1.call_();
  Sub::printStacks();

  sub2.call_();
  Sub::printStacks();

  Sub::clear();
  NameRegistry::clear();
}

void test_caller_higher_depth() {
  std::cout << "Testing calling subprograms at higher depths (more nesting, calling a static parent)." << std::endl;
  std::cout << "Calls at higher depths should work, resulting in no errors.\n" << std::endl;

  Sub mainFunc("main_", nullptr, 0, 0);
  Sub sub1("sub1", &mainFunc, 1, 2);
  Sub sub2("sub2", &mainFunc, 0, 0);
  Sub sub3("sub3", &sub2, 0, 0);

  // This worked in the text_caller_lower_depth example.
  mainFunc.call_();
  sub2.call_();
  sub3.call_();
  Sub::printStacks();

  sub1.call_();
  Sub::printStacks();

  Sub::clear();
  NameRegistry::clear();
}

void test_missing_parent() {
  std::cout << "Testing calling a subprogram that is at a lower depth, and is missing its parent in the stack." << std::endl;
  std::cout << "This should throw an error, as the static parent is missing.\n" << std::endl;

  Sub subMain("main_", nullptr, 0, 0);
  Sub subSub1("sub1", &subMain, 0, 0);
  Sub subSub2("sub2", &subMain, 0, 0);
  Sub subSub3("sub3", &subSub2, 0, 0);

  try {
    subMain.call_();
    subSub1.call_();
    Sub::printStacks();

    subSub3.call_();
    Sub::printStacks();
  } catch (std::runtime_error &e) {
    std::cout << "Failed: " << e.what() << std::endl;
    std::cout << "This was expected behavior.\n" << std::endl;
    Sub::clear();
    NameRegistry::clear();
  }
}

void test_return_depth() {
  std::cout << "Testing returning in reverse order of calls." << std::endl;
  std::cout << "Returning in the same order should work properly, resulting in no errors.\n" << std::endl;

  Sub subMain("main_", nullptr, 0, 0);
  Sub subSub1("sub1", &subMain, 0, 0);
  Sub subSub2("sub2", &subMain, 0, 0);
  Sub subSub3("sub3", &subSub2, 0, 0);

  subMain.call_();
  subSub2.call_();
  subSub3.call_();
  subSub1.call_();
  Sub::printStacks();

  subSub1.return_();
  Sub::printStacks();

  subSub3.return_();
  Sub::printStacks();

  subSub2.return_();
  Sub::printStacks();

  subMain.return_();
  Sub::printStacks();

  Sub::clear();
  NameRegistry::clear();
}

void test_return_depth_error() {
  std::cout << "Testing returning in the wrong order of calls." << std::endl;
  std::cout << "Returning in the wrong order should throw an error, resulting in an exception.\n" << std::endl;

  Sub subMain("main_", nullptr, 0, 0);
  Sub subSub1("sub1", &subMain, 0, 0);
  Sub subSub2("sub2", &subMain, 0, 0);
  Sub subSub3("sub3", &subSub2, 0, 0);

  subMain.call_();
  subSub2.call_();
  subSub3.call_();
  subSub1.call_();
  Sub::printStacks();

  try {
    std::cout << "Attempting to return sub3 before sub1... ";
    subSub3.return_();
  } catch (std::runtime_error &e) {
    std::cout << "Failed: " << e.what() << std::endl;
    std::cout << "This was expected behavior.\n" << std::endl;
    Sub::clear();
    NameRegistry::clear();
  }
}
