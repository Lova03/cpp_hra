#include "utils.h"
#include "Game.h"
#include <random>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <utility>
#include <iostream>
#include <stdexcept>
#include <algorithm>
#include <cctype>

SDL_Texture *loadTexture(const char *filename)
{
  SDL_Surface *surface = IMG_Load(filename);
  if (surface == nullptr)
  {
    return nullptr;
  }
  SDL_Texture *texture = SDL_CreateTextureFromSurface(Game::renderer, surface);
  SDL_FreeSurface(surface);
  return texture;
}

int randomInt(int min, int max)
{
  static std::random_device rd;
  static std::mt19937 mt(rd());
  std::uniform_int_distribution<int> dist(min, max);
  return dist(mt);
}

int randomTileType()
{
  int randNum = randomInt(0, 99);
  if (randNum < 70)
    return 0;
  else if (randNum < 90)
    return 1;
  else
    return 2;
}

/**
 * @brief Parse a price pair from a string.
 *
 * This function parses a price pair in the format "crystalPrice;woodPrice" from a given string.
 * It returns a pair of integers representing the crystal and wood prices respectively.
 *
 * @param str The string containing the price pair.
 * @return std::pair<int, int> The parsed price pair.
 * @throws std::runtime_error if the price pair format is invalid or the prices are not valid numbers.
 */
std::pair<int, int> parsePricePair(const std::string &str)
{
  size_t semicolonPos = str.find(';');
  if (semicolonPos == std::string::npos)
  {
    throw std::runtime_error("Invalid price pair format: missing semicolon");
  }

  std::string crystalPriceStr = str.substr(0, semicolonPos);
  std::string woodPriceStr = str.substr(semicolonPos + 1);

  int crystalPrice, woodPrice;

  try
  {
    crystalPrice = std::stoi(crystalPriceStr);
  }
  catch (const std::invalid_argument &)
  {
    throw std::runtime_error("Invalid price pair format: crystal price is not a number");
  }

  try
  {
    woodPrice = std::stoi(woodPriceStr);
  }
  catch (const std::invalid_argument &)
  {
    throw std::runtime_error("Invalid price pair format: wood price is not a number");
  }

  return std::make_pair(crystalPrice, woodPrice);
}

void loadGameConfig(const std::string &filePath)
{
  std::ifstream configFile(filePath);

  if (configFile.is_open())
  {
    std::string line;
    std::string section;

    while (std::getline(configFile, line))
    {

      // Remove carriage return ("\r") if it exists at the end of the line
      if (!line.empty() && line.back() == '\r')
      {
        line.pop_back();
      }

      if (line.substr(0, 2) == "//" || line.empty())
        continue;

      if (line[0] == '[')
      {
        section = line.substr(1, line.size() - 2);
        continue;
      }

      std::istringstream iss(line);
      std::string itemName, itemValue;

      std::getline(iss, itemName, ',');
      std::getline(iss, itemValue, ',');

      if (!itemName.empty() && !itemValue.empty())
      {
        if (section == "Levels")
        {
          Game::levels.push_back(std::make_pair(itemName, itemValue));
        }
        else if (section == "Talents")
        {
          try
          {
            std::pair<int, int> prices = parsePricePair(itemValue);

            std::string lowercaseItemName = itemName;
            std::transform(lowercaseItemName.begin(), lowercaseItemName.end(), lowercaseItemName.begin(), ::tolower);

            auto it = std::find_if(Game::talents.begin(), Game::talents.end(),
                                   [&](const auto &pair)
                                   {
                                     return pair.first == lowercaseItemName;
                                   });

            if (it != Game::talents.end())
            {
              printf("Error: duplicate talent: %s\n", itemName.c_str());
              Game::isRunning = false;
              return;
            }

            Game::talents.push_back(std::make_pair(lowercaseItemName, prices));
          }
          catch (const std::runtime_error &e)
          {
            printf("Error parsing talent prices: %s\n", e.what());
            Game::isRunning = false;
            return;
          }
        }
      }
    }

    configFile.close();
  }
  else
  {
    printf("Unable to open config file: %s", filePath.c_str());
    Game::isRunning = false;
  }
}

std::pair<std::string, std::string> getSoldierTexturePath(int id)
{
  int index = id % 6;

  switch (index)
  {
  case 0:
    return {"./assets/blue_soldier.png", "./assets/blue_soldier_selected.png"};
    break;
  case 1:
    return {"./assets/red_soldier.png", "./assets/red_soldier_selected.png"};
    break;
  case 2:
    return {"./assets/green_soldier.png", "./assets/green_soldier_selected.png"};
    break;
  case 3:
    return {"./assets/yellow_soldier.png", "./assets/yellow_soldier_selected.png"};
    break;
  case 4:
    return {"./assets/purple_soldier.png", "./assets/purple_soldier_selected.png"};
    break;
  default:
    return {"./assets/orange_soldier.png", "./assets/orange_soldier_selected.png"};
  }
}

std::string getCastleTexturePath(int id)
{
  int index = id % 6;

  switch (index)
  {
  case 0:
    return "./assets/blue_castle.png";
    break;
  case 1:
    return "./assets/red_castle.png";
    break;
  case 2:
    return "./assets/green_castle.png";
    break;
  case 3:
    return "./assets/yellow_castle.png";
    break;
  case 4:
    return "./assets/purple_castle.png";
    break;
  default:
    return "./assets/orange_castle.png";
  }
}

std::pair<std::string, std::string> getWorkerTexturePath(int id)
{
  int index = id % 6;

  switch (index)
  {
  case 0:
    return {"./assets/blue_worker.png", "./assets/blue_worker_selected.png"};
    break;
  case 1:
    return {"./assets/red_worker.png", "./assets/red_worker_selected.png"};
    break;
  case 2:
    return {"./assets/green_worker.png", "./assets/green_worker_selected.png"};
    break;
  case 3:
    return {"./assets/yellow_worker.png", "./assets/yellow_worker_selected.png"};
    break;
  case 4:
    return {"./assets/purple_worker.png", "./assets/purple_worker_selected.png"};
    break;
  default:
    return {"./assets/orange_worker.png", "./assets/orange_worker_selected.png"};
  }
}
