#ifndef ASTAR_H
#define ASTAR_H

#include "Map.h"
#include <vector>
#include <utility>

/**
 * @class AStar
 * @brief A* Pathfinding Algorithm
 *
 * The AStar class is responsible for finding the shortest path between two points on a game map using the A* algorithm.
 * It takes into account the obstacles and terrain of the map to determine the most optimal path.
 */
class AStar
{
public:
  /**
   * @brief Constructor for the AStar class.
   * Initializes the AStar object with a reference to the game map.
   * @param map The game map to be used for pathfinding.
   */
  AStar(Map &map);

  /**
   * @brief Destructor for the AStar class.
   * Cleans up the AStar object upon deletion.
   */
  ~AStar();

  /**
   * @brief Finds a path from one point to another.
   * Uses the A* pathfinding algorithm to find the shortest path from one point to another on the game map.
   * The returned path is a vector of coordinates from the start to the goal.
   * If no path can be found, returns an empty vector.
   * @param startX The x coordinate of the start position.
   * @param startY The y coordinate of the start position.
   * @param goalX The x coordinate of the goal position.
   * @param goalY The y coordinate of the goal position.
   * @return A vector of coordinate pairs representing the path, or an empty vector if no path is found.
   */
  std::vector<std::pair<int, int>> getPath(int startX, int startY, int goalX, int goalY);

private:
  Map &map;
};

#endif
