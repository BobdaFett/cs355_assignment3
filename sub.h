//
// Created by lvas4_0gev1ym on 12/2/2025.
//

#ifndef CS355_ASSIGNMENT3_SUBPROGRAM_H
#define CS355_ASSIGNMENT3_SUBPROGRAM_H
#include <string>
#include <vector>

class Sub {
public:
  /// Constructor. Creates a new Sub with the given name, static parent, number of params and number
  /// of local variables.
  Sub(const std::string& name, Sub *staticParent, int numParams,
      int numLocals);

  /// "Calls" this subprogram. It will simply create a stack frame for it and
  /// push it to the stack.
  void call_() const;
  /// "Returns" this subprogram. It will remove the stack frame from the stack.
  void return_() const;

  /// Prints the current contents of the stack and display vectors in a
  /// user-friendly format.
  static void printStacks(const std::string &message);
  /// Clears all information from the runtime and display stacks.
  static void clear();

protected:
  /// Pushes an ARI for this Sub to the stack. Returns the first index of the
  /// ARI in the stack, which can be used to update the display.
  int pushAriToStack() const;

  /// Checks if the name of this Sub is visible to the current caller.
  [[nodiscard]] bool isNameVisible() const;

  /// Checks if this Sub was the most recently called.
  [[nodiscard]] bool isMostRecentCaller() const;

  /// The runtime stack. Stores all the ARIs.
  static std::vector<int> stack;
  /// The display vector. Indexing into this vector should use the static depth,
  /// which returns the location of the bottom of the stack frame for the Sub at
  /// the given depth.
  static std::vector<int> display;
  /// A collection containing the locations of all names in the stack. This is only to simplify logging
  /// and is not used anywhere else (that would be cheating). It is sorted in the order that Subs are added
  /// to the stack.
  static std::vector<int> nameLocations;

  /// Used to keep track of what the most recently called Sub is. Since the names in our runtime stack
  /// are only for show and cannot be used, this is the only way to determine when a function is in scope.
  /// Subs are pushed in the order that they are called, and then popped as they return, and as such the
  /// most recently called Sub is at the back of the list.
  static std::vector<const Sub*> callers;

  /// The name of this Sub.
  std::string name;
  /// The static parent of this Sub. A top-level Sub has no parent, and therefore may contain a nullptr.
  Sub *staticParent;
  /// The list of all this Sub's children. The list may be empty.
  std::vector<Sub *> children;
  /// The number of params that this Sub has. Negative numbers are ignored, and parameters are created without
  /// specific values (calling the function will always take no real arguments).
  int numParams;
  /// The number of local variables that this Sub has. Negative numbers are ignored. These variables
  /// do not have any real values (functions do not have a real implementation, and so the values are not used).
  int numLocals;

  /// This subprogram's static depth in the display.
  int staticDepth;
};

#endif // CS355_ASSIGNMENT3_SUBPROGRAM_H
