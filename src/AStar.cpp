#include "Game.h"
#include "AStar.h"
#include <set>
#include <map>
#include <algorithm>
#include <stdexcept>
#include <cmath>

AStar::AStar(Map &map) : map(map) {}

AStar::~AStar() = default;

/**
 * @brief Node structure for the AStar class.
 * Represents a node in the pathfinding graph, with x, y coordinates and g, h, f costs.
 * The 'cameFrom' shared pointer points to the previous node in the path.
 */
struct Node
{
  int x, y;
  float g, h, f;
  std::shared_ptr<Node> cameFrom;

  Node(int x, int y, std::shared_ptr<Node> cameFrom = nullptr) : x(x), y(y), g(std::numeric_limits<float>::infinity()), h(0), f(0), cameFrom(cameFrom) {}

  // Compare nodes based on f value for set
  bool operator<(const Node &other) const
  {
    if (f != other.f)
    {
      return f < other.f;
    }
    if (x != other.x)
    {
      return x < other.x;
    }
    return y < other.y;
  }
};

/**
 * @brief Functor for comparing nodes.
 * Used to compare nodes based on their f values in the open set of the AStar algorithm.
 */
struct compareF
{
  bool operator()(const std::shared_ptr<Node> &a, const std::shared_ptr<Node> &b) const
  {
    return *a < *b;
  }
};

std::vector<std::pair<int, int>> AStar::getPath(int startX, int startY, int goalX, int goalY)
{
  std::set<std::shared_ptr<Node>, compareF> openSet;
  std::map<std::pair<int, int>, std::shared_ptr<Node>> allNodes;

  std::shared_ptr<Node> startNode = std::make_shared<Node>(startX, startY);
  startNode->g = 0;
  startNode->h = std::abs(goalX - startX) + std::abs(goalY - startY); // Manhatten distance
  startNode->f = startNode->g + startNode->h;

  allNodes[{startX, startY}] = startNode;
  openSet.insert(startNode);

  while (!openSet.empty())
  {
    std::shared_ptr<Node> current = *openSet.begin();
    openSet.erase(openSet.begin());

    if (current->x == goalX && current->y == goalY)
    {
      std::vector<std::pair<int, int>> path;
      while (current)
      {
        path.push_back({current->x, current->y});
        current = current->cameFrom;
      }
      std::reverse(path.begin(), path.end());
      return path;
    }

    for (int dx = -1; dx <= 1; ++dx)
    {
      for (int dy = -1; dy <= 1; ++dy)
      {
        if (dx == 0 && dy == 0)
          continue;

        int nextX = current->x + dx;
        int nextY = current->y + dy;

        // Diagonal movement check
        if (dx != 0 && dy != 0)
        {
          if (!map.isAccessible(current->x, current->y + dy) || !map.isAccessible(current->x + dx, current->y))
          {
            continue;
          }
        }

        if (map.isAccessible(nextX, nextY))
        {
          std::shared_ptr<Node> successor;

          if (allNodes.count({nextX, nextY}))
          {
            successor = allNodes[{nextX, nextY}];
          }
          else
          {
            successor = std::make_shared<Node>(nextX, nextY, current);
            allNodes[{nextX, nextY}] = successor;
          }

          float tentative_g = current->g + std::sqrt(dx * dx + dy * dy);

          if (tentative_g < successor->g)
          {
            openSet.erase(successor);
            successor->cameFrom = current;
            successor->g = tentative_g;
            successor->h = std::abs(goalX - nextX) + std::abs(goalY - nextY);
            successor->f = successor->g + successor->h;
            openSet.insert(successor);
          }
        }
      }
    }
  }

  // No path found
  return {};
}
