//
// Created by lvas4_0gev1ym on 12/2/2025.
//

#ifndef CS355_ASSIGNMENT3_SUBPROGRAM_H
#define CS355_ASSIGNMENT3_SUBPROGRAM_H
#include <string>
#include <utility>
#include <vector>

class Sub {
public:
  Sub(std::string name, Sub *staticParent, const int numParams,
      const int numLocals);

  /// "Calls" this subprogram. It will simply create a stack frame for it and
  /// push it to the stack.
  void call_();
  /// "Returns" this subprogram. It will remove the stack frame from the stack.
  void return_();

  /// Prints the current contents of the stack and display vectors in a
  /// user-friendly format.
  static void printStacks();

protected:
  /// Pushes an ARI for this Sub to the stack. Returns the first index of the
  /// ARI in the stack, which can be used to update the display.
  int pushAriToStack();

  /// Checks if the name of this Sub is visible by checking all static
  /// ancestors.
  bool isNameVisible();

  /// The runtime stack. Stores all the ARIs.
  static std::vector<int> stack;
  /// The display vector. Indexing into this vector should use the static depth,
  /// which returns the location of the bottom of the stack frame for the Sub at
  /// the given depth.
  static std::vector<int> display;

  std::string name;
  Sub *staticParent;
  std::vector<Sub *> children;
  int numParams;
  int numLocals;

  /// This subprogram's static depth in the display.
  int staticDepth;
};

#endif // CS355_ASSIGNMENT3_SUBPROGRAM_H
