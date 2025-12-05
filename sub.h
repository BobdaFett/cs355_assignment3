//
// Created by lvas4_0gev1ym on 12/2/2025.
//

#ifndef CS355_ASSIGNMENT3_SUBPROGRAM_H
#define CS355_ASSIGNMENT3_SUBPROGRAM_H
#include <string>
#include <vector>

class Sub {
public:
  Sub(const std::string& name, Sub *staticParent, int numParams,
      int numLocals);

  /// "Calls" this subprogram. It will simply create a stack frame for it and
  /// push it to the stack.
  void call_() const;
  /// "Returns" this subprogram. It will remove the stack frame from the stack.
  void return_() const;

  /// Prints the current contents of the stack and display vectors in a
  /// user-friendly format.
  static void printStacks();
  /// Clears all information from the runtime and display stacks.
  static void clear();

protected:
  /// Pushes an ARI for this Sub to the stack. Returns the first index of the
  /// ARI in the stack, which can be used to update the display.
  int pushAriToStack() const;

  /// Checks if the name of this Sub is visible by checking all static
  /// ancestors.
  bool isNameVisible() const;

  /// Checks if this Sub is the ARI currently at the top of the stack.
  bool isTopOfStack() const;

  /// The runtime stack. Stores all the ARIs.
  static std::vector<int> stack;
  /// The display vector. Indexing into this vector should use the static depth,
  /// which returns the location of the bottom of the stack frame for the Sub at
  /// the given depth.
  static std::vector<int> display;
  /// A collection containing the locations of all names in the stack. This is only for logging purposes
  /// and is not used anywhere else (that would be cheating). It is sorted in the order that Subs are added
  /// to the stack.
  static std::vector<int> nameLocations;

  std::string name;
  Sub *staticParent;
  std::vector<Sub *> children;
  int numParams;
  int numLocals;

  /// This subprogram's static depth in the display.
  int staticDepth;
};

#endif // CS355_ASSIGNMENT3_SUBPROGRAM_H
