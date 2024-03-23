#include "Map.h"
#include "AStar.h"
#include <utility>
#include <list>

Map::Map() = default;

Map::~Map() = default;

std::list<std::pair<int, int>> Map::calculatePath(std::pair<int, int> startCoords, std::pair<int, int> targetCoords)
{
  AStar pathFinder(*this);                                                                                                                       // Vytvoříme instanci AStar s danou mapou
  std::vector<std::pair<int, int>> pathVec = pathFinder.getPath(startCoords.first, startCoords.second, targetCoords.first, targetCoords.second); // Vypočteme cestu
  std::list<std::pair<int, int>> path(pathVec.begin(), pathVec.end());                                                                           // Převedeme vektor na list
  return path;
}

void Map::load(const std::vector<std::string> &mapData)
{
  int height = mapData.size();
  int width = mapData[0].length();
  grid = std::vector<std::vector<bool>>(height, std::vector<bool>(width, true));

  for (int y = 0; y < height; ++y)
  {
    for (int x = 0; x < width; ++x)
    {
      if (mapData[y][x] == 'C' || mapData[y][x] == 'T' || mapData[y][x] == 'W' || mapData[y][x] == 'P' || mapData[y][x] == 'X')
      {
        grid[y][x] = false;
      }
    }
  }
}

bool Map::isAccessible(int x, int y) const
{
  // Check that coordinates are within the grid bounds
  if (x < 0 || x >= 50 || y < 0 || y >= 32)
  {
    return false;
  }

  return grid[y][x];
}
