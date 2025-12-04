//
// Created by lvas4_0gev1ym on 12/2/2025.
//

#include "sub.h"
#include "nameRegistry.h"
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <stdexcept>

std::vector<int> Sub::stack{};
std::vector<int> Sub::display{};

Sub::Sub(std::string name, Sub *staticParent, const int numParams,
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

void Sub::call_() {
  // Check if this is the main program or if the name is visible.
  if (name == "main_" || isNameVisible()) {
    int index = pushAriToStack();

    // Update the display stack with the current Sub's ARI index
    // If the depth doesn't exist, create it.
    if (display.size() > staticDepth) {
      display[staticDepth] = index;
    } else {
      // It's a prerequisite that the depth cannot be more than one
      // greater than the current depth, so increasing it by one is safe.
      display.push_back(index);
    }
  } else {
    throw std::runtime_error("Subprogram with name " + name +
                             " is not visible.");
  }
}

bool Sub::isNameVisible() {
  // Check this Sub's static ancestors for a child with the same name.
  // If there is no child with the same name, return false.
  Sub *checkParent = staticParent;
  while (checkParent != nullptr) {
    for (Sub *child : staticParent->children) {
      if (child->name == name) {
        return true;
      }
    }

    checkParent = checkParent->staticParent;
  }

  return false;
}

int Sub::pushAriToStack() {
  // Store the name in a map that contains the names of all current
  // subprograms. This might be better to store as a member variable, double
  // check the assignment. First, get the name's index.
  int nameIdx = NameRegistry::getIndex(name);
  // Push the name index to the stack. We can retrieve the name from the index
  // when needed.
  stack.push_back(nameIdx);
  // Save the current value stored in the display at this Sub's static depth.
  // A value of -1 indicates that there was no previous data at this depth.
  int displayLink = display.size() > staticDepth ? display[staticDepth] : -1;
  stack.push_back(displayLink);
  // Save the current dynamic link, which is the index at the former top
  // element of the stack.
  int dynLink = stack.size() > 0 ? stack.size() - 1 : 0;
  stack.push_back(dynLink);
  // Push nonsense values, one for each parameter.
  int numArgsLocals = numParams + numLocals;
  for (int i = 0; i < numArgsLocals; i++) {
    stack.push_back(i);
  }

  for (int i : stack) {
    std::cout << i << " ";
  }
  std::cout << std::endl;

  // The first index of the ARI in the stack is the name of the Sub.
  return nameIdx;
}

void Sub::return_() {
  // Remove ARI of this Sub from the stack and reflect in the display.
  int numArgsLocals = numParams + numLocals;
  for (int i = 0; i < numArgsLocals; i++)
    stack.pop_back(); // pop all locals and parameters
  int dynLink = stack.back();
  stack.pop_back(); // pop dynamic link
  int displayLink = stack.back();
  stack.pop_back(); // pop display link
  stack.pop_back(); // pop name index

  // Restore display link
  display[staticDepth] = displayLink;

  // Restore dynamic link (do we actually need to do anything with this?)
  // The dynamic link is not used anywhere else.
}

void Sub::printStacks() {
  // We want to organize all information into a simple table containing two
  // columns - one for the stack and one for the display. The display vector
  // will not print the value of the display at a specific index, but will
  // print the index at the corresponding location in the stack (the display
  // stored 3 at index 1, so we will print 1 at row 3).

  // First, find formatting information. This includes the maximum width of
  // each of the stack and display elements, including mapping the names of
  // each Sub in the stack and the column headers. We'll convert everything
  // into strings for easier formatting later.
  std::string stackHeader = "Stack", displayHeader = "Display";
  int maxStackWidth = stackHeader.size(),
      maxDisplayWidth = displayHeader.size();
  std::vector<std::string> stackValues, displayValues;

  for (int stackValue : stack) {
    std::string stackValueStr = std::to_string(stackValue);
    stackValues.push_back(stackValueStr);
    maxStackWidth =
        std::max(maxStackWidth, static_cast<int>(stackValueStr.size()));
  }

  for (int displayValue : display) {
    std::string displayValueStr = std::to_string(displayValue);
    displayValues.push_back(displayValueStr);
    maxDisplayWidth =
        std::max(maxDisplayWidth, static_cast<int>(displayValueStr.size()));
  }

  // We also need to check the names registry to determine the maximum width
  // of the names
  maxStackWidth = std::max(maxStackWidth, NameRegistry::getMaxWidth());

  if (stackValues.size() == 0) {
    std::cout << "Empty stack." << std::endl;
    return;
  }

  // Print bar above headers - sum of all widths, plus 2 for each edge bar
  // and 3 for each column bar
  std::string horizontalBar =
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
  int displayIdx = 0;
  for (int i = 0; i < stackValues.size(); i++) {

    // Start edge bar
    std::cout << " | ";

    // Determine stack value and display value
    std::cout << "Value stored at display index " << displayIdx << ": "
              << display[displayIdx] << std::endl;
    if (i == display[displayIdx]) { // We should also resolve the name
      std::cout << std::setw(maxStackWidth)
                << NameRegistry::getName(displayIdx);
      std::cout << " | ";
      std::cout << std::setw(maxDisplayWidth) << displayIdx;
      displayIdx++;
    } else {
      std::cout << std::setw(maxStackWidth) << stackValues[i];
      std::cout << " | ";
      std::cout << std::setw(maxDisplayWidth) << "";
    }

    // End edge bar
    std::cout << " | " << std::endl;
  }

  // Print bar below values, same as top bar.
  std::cout << horizontalBar << std::endl;
}
