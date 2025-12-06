//
// Created by lvas4_0gev1ym on 12/2/2025.
//

#include "sub.h"
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include "nameRegistry.h"

std::vector<int> Sub::stack{};
std::vector<int> Sub::display{};
std::vector<int> Sub::nameLocations{};
std::vector<const Sub*> Sub::callers{};

Sub::Sub(const std::string& name, Sub *staticParent, const int numParams,
         const int numLocals)
    : name(name), staticParent(staticParent), numParams(numParams),
      numLocals(numLocals), staticDepth(0) {
  // Calculate and initialize the static depth.
  // This means we need to find how many parents this Sub has.
  const Sub *nextParent = staticParent;
  while (nextParent != nullptr) {
    staticDepth++;
    nextParent = nextParent->staticParent;
  }

  if (staticParent != nullptr) {
    staticParent->children.push_back(this);
  }

  NameRegistry::addName(name);
}

void Sub::call_() const {
  // Check if this is the main program or if the name is visible.
  if (stack.empty() && name != "main_") throw std::runtime_error("Must call function main_ first.");
  if (name == "main_" || isNameVisible()) {
    const int index = pushAriToStack();

    // Update the display stack with the current Sub's ARI index
    // If the depth doesn't exist, create it.
    if (staticDepth < display.size()) {
      display[staticDepth] = index;
    } else {
      // It's a prerequisite that the depth cannot be more than one
      // greater than the current depth, so increasing it by one is safe.
      display.push_back(index);
    }
    callers.push_back(this);
  } else {
    throw std::runtime_error("Subprogram with name " + name +
                             " is not visible, as static parent " + staticParent->name +
                             " is not currently in the display.");
  }
}

bool Sub::isNameVisible() const {
  // This function is only visible if its static parent is contained in the stack and is covered
  // by the display (i.e. we check the stack indices indicated by the display and check the names at those locations)

  // Use the currentCaller pointer
  const Sub* checkCaller = !callers.empty() ? callers.back() : nullptr;
  while (checkCaller != nullptr) {
    // Loop and check the current caller's children to see if the function is visible.
    for (const Sub* child : checkCaller->children) {
      if (child->name == name) return true;
    }
    checkCaller = checkCaller->staticParent;
  }

  return false;
}

bool Sub::isMostRecentCaller() const {
  return callers.back()->name == name;
}

int Sub::pushAriToStack() const {
  // Store the name in a map that contains the names of all current
  // subprograms. This might be better to store as a member variable, double
  // check the assignment. First, get the name's index.
  const int nameIdx = NameRegistry::getIndex(name);
  // Next, get the current size of the stack - that will be the dynamic link.
  const int displayIdx = stack.size();
  const int dynLink = stack.size() - 1;
  // Push the name index to the stack. We can retrieve the name from the index
  // when needed.
  stack.push_back(nameIdx);
  nameLocations.push_back(displayIdx);
  // Save the current value stored in the display at this Sub's static depth.
  // A value of -1 indicates that there was no previous data at this depth.
  const int displayLink = staticDepth < display.size() ? display[staticDepth] : -1;
  stack.push_back(displayLink);
  // Save the current dynamic link, which is the index at the former top
  // element of the stack.
  stack.push_back(dynLink);
  // Push nonsense values, one for each parameter.
  const int numArgsLocals = numParams + numLocals;
  for (int i = 0; i < numArgsLocals; i++) {
    stack.push_back(i);
  }

  // The first index of the ARI in the stack is the name of the Sub.
  return displayIdx;
}

void Sub::return_() const {
  // We first need to check that this Sub is the top of the stack.
  // If it's not, we need to throw an error.
  if (!isMostRecentCaller()) {
    throw std::runtime_error("Cannot return from subprogram " + name +
                             " as it is not at the top of the stack.");
  }

  // Remove ARI of this Sub from the stack and reflect in the display.
  const int numArgsLocals = numParams + numLocals;
  for (int i = 0; i < numArgsLocals; i++)
    stack.pop_back(); // pop all locals and parameters
  stack.pop_back(); // pop dynamic link
  const int displayLink = stack.back();
  stack.pop_back(); // pop display link
  stack.pop_back(); // pop name index
  nameLocations.pop_back(); // pop name location from the list

  // Restore display link
  display[staticDepth] = displayLink;

  // Pop the Sub pointer from the stack - it's no longer the executing function.
  // This maintains the scope for all other functions in the stack, if any.
  callers.pop_back();
}

void Sub::printStacks(const std::string &message) {
  // We want to organize all information into a simple table containing two
  // columns - one for the stack and one for the display. The display column
  // will not print the value of the display at a specific index, but will
  // print the index at the corresponding location in the stack (the display
  // stored 3 at index 1, so we will print 1 at row 3).

  // First, find formatting information. This includes the maximum width of
  // each of the stack and display elements, including mapping the names of
  // each Sub in the stack and the column headers. We'll convert everything
  // into strings for easier formatting later.
  const std::string stackHeader = "Stack", displayHeader = "Display";
  int maxStackWidth = stackHeader.size(),
      maxDisplayWidth = displayHeader.size();
  std::vector<std::string> stackValues, displayValues;

  for (const int stackValue : stack) {
    std::string stackValueStr = std::to_string(stackValue);
    stackValues.push_back(stackValueStr);
    maxStackWidth =
        std::max(maxStackWidth, static_cast<int>(stackValueStr.size()));
  }

  for (const int displayValue : display) {
    std::string displayValueStr = std::to_string(displayValue);
    displayValues.push_back(displayValueStr);
    maxDisplayWidth =
        std::max(maxDisplayWidth, static_cast<int>(displayValueStr.size()));
  }

  // We also need to check the name registry to determine the maximum width
  // of the names
  maxStackWidth = std::max(maxStackWidth, NameRegistry::getMaxWidth());

  // All stack messages print with a message first.
  std::cout << message << std::endl;

  if (stackValues.empty()) {
    std::cout << "Empty stack." << std::endl;
    return;
  }

  // Print bar above headers - sum of all widths, plus 2 for each edge bar
  // and 3 for each column bar
  const std::string horizontalBar =
      " " + std::string(maxStackWidth + maxDisplayWidth + 7, '-');
  std::cout << horizontalBar << std::endl;

  // Print headers
  // Edge bar
  std::cout << " | ";
  // Stack header
  std::cout << std::setw(maxStackWidth) << stackHeader;
  // Column bar
  std::cout << " | ";
  // Display header
  std::cout << std::setw(maxDisplayWidth) << displayHeader;
  // Edge bar
  std::cout << " | " << std::endl;

  // Print bar below headers.
  std::cout << horizontalBar << std::endl;

  // Log the values.
  int nameIdx = nameLocations.size() - 1,
      maxDisplayIdx = display.size() - 1;
  for (int i = stackValues.size() - 1; i >= 0; i--) {

    // Start edge bar
    std::cout << " | " << std::setw(maxStackWidth);

    // Determine whether we print a number or a function name
    if (nameIdx < nameLocations.size() && i == nameLocations[nameIdx]) {
      std::cout << NameRegistry::getName(stack[i]);
      nameIdx--;
    } else {
      std::cout << stackValues[i];
    }

    std::cout << " | " << std::setw(maxDisplayWidth);

    // Determine whether we should print the display value
    bool usedDisplay = false;
    for (int d = maxDisplayIdx; d >= 0; d--) {
      if (i == display[d]) {
        std::cout << d;
        usedDisplay = true;
        maxDisplayIdx = d - 1;
        break;
      }
    }

    if (!usedDisplay) {
      std::cout << "";
    }

    // End edge bar
    std::cout << " | " << std::endl;
  }

  // Print bar below values, same as top bar.
  std::cout << horizontalBar << std::endl;
}

void Sub::clear() {
  stack.clear();
  display.clear();
  nameLocations.clear();
  callers.clear();
}
