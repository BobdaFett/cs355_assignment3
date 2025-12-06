#include <iostream>

#include "nameRegistry.h"
#include "sub.h"

void test_call_non_main();
void test_equal_depths();
void test_caller_lower_depth();
void test_caller_higher_depth();
void test_missing_parent();
void test_return_depth();
void test_return_depth_error();
void test_args_call_return();

int main() {
  try {
    test_call_non_main();
    test_equal_depths();
    test_caller_lower_depth();
    test_caller_higher_depth();
    test_missing_parent();
    test_return_depth();
    test_return_depth_error();
    test_args_call_return();
  } catch (std::runtime_error &e) {
    std::cout << "An unexpected error occurred: " << e.what() << std::endl;
    std::cout << "Exiting..." << std::endl;
    return 1;
  }

  return 0;
}

void test_call_non_main() {
  std::cout << "Testing calling a subprogram that is not the main program first." << std::endl;
  std::cout << "This should throw an error, as one of the rules is that a function named" << std::endl;
  std::cout << "\"main_\" must be the first function called.\n" << std::endl;

  Sub subMain("main_", nullptr, 0, 0);
  Sub subSub("sub_", &subMain, 0, 0);

  try {
    Sub::printStacks("Stacks before function call:");

    std::cout << "Attempting to call sub_ before main_... ";
    subSub.call_();
  } catch (std::runtime_error &e) {
    std::cout << "Failed: " << e.what() << std::endl;
    std::cout << "This was expected behavior.\n" << std::endl;
  }

  Sub::clear();
  NameRegistry::clear();
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
  Sub::printStacks("Stacks after function calls:");

  Sub::clear();
  NameRegistry::clear();
}

void test_caller_lower_depth() {
  std::cout << "Testing calling a subprogram that is at a lower depth (less nesting, calling a child)." << std::endl;
  std::cout << "Calls at lower depths should work, resulting in no errors.\n" << std::endl;

  Sub mainFunc("main_", nullptr, 0, 0);
  Sub sub1("sub1", &mainFunc, 0, 0);
  Sub sub2("sub2", &sub1, 0, 0);

  mainFunc.call_();
  sub1.call_();
  Sub::printStacks("Stacks after initial function calls:");

  sub2.call_();
  Sub::printStacks("Stacks after nested function call:");

  std::cout << "There was no error, so this call was successful.\n" << std::endl;

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
  Sub::printStacks("Stacks after initial function calls:");

  sub1.call_();
  Sub::printStacks("Stacks after function call to a higher depth:");

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
    Sub::printStacks("Stacks after initial function calls:");

    std::cout << "Attempting to call sub3 from sub1... ";
    subSub3.call_();
    Sub::printStacks("Stacks after sub3 call (this should not print):");
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
  Sub::printStacks("Stacks after initial function calls:");

  subSub1.return_();
  Sub::printStacks("Stacks after sub1 return:");

  subSub3.return_();
  Sub::printStacks("Stacks after sub3 return:");

  subSub2.return_();
  Sub::printStacks("Stacks after sub2 return:");

  subMain.return_();
  Sub::printStacks("Stacks after main return:");

  std::cout << "\n";

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
  Sub::printStacks("Stacks after initial function calls:");

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

void test_args_call_return() {
  std::cout << "Testing calling functions that have arguments and local variables to ensure " << std::endl;
  std::cout << "that the stack is properly updated." << std::endl;
  std::cout << "We'll define three functions - main_ has no locals/params, sub1 has 1 param and 2 locals," << std::endl;
  std::cout << "and sub2 has 4 params and 10 locals.\n" << std::endl;

  Sub subMain("main_", nullptr, 0, 0);
  Sub subSub("sub_", &subMain, 1, 2);
  Sub subSub2("sub2", &subMain, 4, 10);

  subMain.call_();
  Sub::printStacks("Stacks after main call:");
  subSub.call_();
  Sub::printStacks("Stacks after sub1 call:");
  subSub2.call_();
  Sub::printStacks("Stacks after sub2 call:");
  subSub2.return_();
  Sub::printStacks("Stacks after sub2 return:");
  subSub.return_();
  Sub::printStacks("Stacks after sub1 return:");
  subMain.return_();
  Sub::printStacks("Stacks after main return:");

  std::cout << "\n";
}
