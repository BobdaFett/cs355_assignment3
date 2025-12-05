#include "nameRegistry.h"
#include <algorithm>
#include <vector>

std::vector<std::string> NameRegistry::names{};

int NameRegistry::addName(const std::string& name) {
  for (int i = 0; i < names.size(); i++) {
    if (names[i].empty()) {
      names[i] = name;
      return i;
    }
  }

  names.push_back(name);
  return names.size() - 1;
}

void NameRegistry::removeName(const std::string& name) {
  for (std::string &i : names) {
    if (i == name) {
      i = "";
      break;
    }
  }
}

std::string NameRegistry::getName(const int index) {
  std::string result;
  if (index < names.size() && index >= 0)
    result = names[index];

  return result;
}

int NameRegistry::getIndex(const std::string& name) {
  int result = -1;
  for (int i = 0; i < names.size(); i++) {
    if (names[i] == name) {
      result = i;
      break;
    }
  }

  return result;
}

int NameRegistry::getMaxWidth() {
  int maxWidth = 0;
  for (const std::string &name : names) {
    maxWidth = std::max(maxWidth, static_cast<int>(name.size()));
  }
  return maxWidth;
}

void NameRegistry::clear() { names.clear(); }
