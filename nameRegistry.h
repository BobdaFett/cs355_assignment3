#ifndef NAME_MAP_H
#define NAME_MAP_H
#include <string>
#include <vector>

class NameRegistry {
public:
  NameRegistry();

  /// Adds the given name to the map. Uses the next available index,
  /// defined as either the smallest index containing an empty string or the
  /// next index after the last element.
  static int addName(const std::string name);
  /// Removes the given name from the map by leaving the index empty. This
  /// means that other indices will not be affected, but the index can still
  /// be reused when a new name is added.
  static void removeName(const std::string name);
  /// Gets the name at the given index, or an empty string if the index is out
  /// of bounds.
  static std::string getName(int index);
  /// Gets the index of the given name in the map, or -1 if not found.
  static int getIndex(const std::string name);
  /// Gets the maximum width of the names in the registry.
  static int getMaxWidth();

protected:
  static std::vector<std::string> names;
};

#endif
