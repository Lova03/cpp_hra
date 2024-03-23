#ifndef MAP_H
#define MAP_H

#include <vector>
#include <string>
#include <list>
#include <utility>

/**
 * @class Map
 * @brief Represents the game map.
 *
 * The Map class is responsible for managing the game map and providing operations related to the map,
 * such as loading map data, checking accessibility of coordinates, and calculating paths using the A* algorithm.
 * It uses a grid representation to store information about each cell in the map.
 */
class Map
{
public:
  /**
   * @brief Default constructor for the Map class.
   */
  Map();

  /**
   * @brief Default destructor for the Map class.
   */
  ~Map();

  /**
   * @brief Loads map data into the map grid.
   *
   * The map data is represented by a vector of strings. Each string represents a row in the map.
   * Different characters in the string represent different objects in the game.
   * If the object is solid it false gets pushed to the grid otherwise true is pushed.
   *
   * @param mapData A vector of strings representing the map data.
   */
  void load(const std::vector<std::string> &mapData);

  /**
   * @brief Checks whether a given coordinate pair (x, y) is accessible in the map.
   *
   * A coordinate pair is considered accessible if it is within the map grid bounds and not blocked by any object.
   *
   * @param x The x-coordinate.
   * @param y The y-coordinate.
   * @return bool True if the coordinate pair is accessible, false otherwise.
   */
  bool isAccessible(int x, int y) const;

  /**
   * @brief Calculates the path between two points using the A* algorithm.
   *
   * @param startCoords The coordinates (x, y) of the starting point.
   * @param targetCoords The coordinates (x, y) of the target point.
   * @return std::list<std::pair<int, int>> A list of coordinates representing the calculated path.
   */
  std::list<std::pair<int, int>> calculatePath(std::pair<int, int> startCoords, std::pair<int, int> targetCoords);

private:
  std::vector<std::vector<bool>> grid;
};

#endif
