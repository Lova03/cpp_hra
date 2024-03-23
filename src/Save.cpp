#include "Save.h"
#include "Game.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <stdexcept>

Save::Save(const std::string &filePath)
    : filePath(filePath)
{
}

Save::~Save() = default;

void Save::init()
{
  for (const auto &level : Game::levels)
  {
    LevelState defaultState;
    defaultState.levelName = level.first;
    levelStates[level.first] = defaultState;
  }
}

LevelState *Save::getLevelState(const std::string &levelName)
{
  auto it = levelStates.find(levelName);
  return it != levelStates.end() ? &it->second : nullptr;
}

void Save::saveLevelState(const LevelState &levelState)
{
  levelStates[levelState.levelName] = levelState;
  save();
}

void Save::load()
{
  std::ifstream file(filePath);
  if (!file.is_open())
  {
    printf("Cannot open save file: %s\n", filePath.c_str());
    return;
  }

  std::string line;
  LevelState currentLevel;
  bool isReadingLevel = false;

  while (std::getline(file, line))
  {

    // Remove carriage return ("\r") if it exists at the end of the line
    if (!line.empty() && line.back() == '\r')
    {
      line.pop_back();
    }

    // Ignore empty lines and comments
    if (line.empty() || (line[0] == '/' && line[1] == '/'))
      continue;

    // Check if line is a level name
    if (line[0] == '[' && line.back() == ']')
    {
      if (isReadingLevel)
      {
        // Save the previous level before moving to the next one
        levelStates[currentLevel.levelName] = currentLevel;
        currentLevel = LevelState();
      }

      currentLevel.levelName = line.substr(1, line.size() - 2);
      isReadingLevel = true;
      continue;
    }

    std::istringstream iss(line);
    std::string type;
    std::getline(iss, type, ',');

    std::string value; // general value holder for conversion

    try
    {
      if (type == "Unit")
      {
        LevelState::UnitInfo unit;

        // ownerId
        std::getline(iss, value, ',');
        unit.ownerId = std::stoi(value);

        // health
        std::getline(iss, value, ',');
        unit.health = std::stoi(value);

        // type
        std::getline(iss, unit.type, ',');

        // coords.first
        std::getline(iss, value, ',');
        unit.coords.first = std::stoi(value);

        // coords.second
        std::getline(iss, value, ',');
        unit.coords.second = std::stoi(value);

        currentLevel.units.push_back(unit);
      }
      else if (type == "Castle")
      {
        LevelState::CastleInfo castle;

        // ownerId
        std::getline(iss, value, ',');
        castle.ownerId = std::stoi(value);

        // health
        std::getline(iss, value, ',');
        castle.health = std::stoi(value);

        currentLevel.castles.push_back(castle);
      }
      else if (type == "Talent")
      {
        LevelState::TalentInfo talent;

        // ownerId
        std::getline(iss, value, ',');
        talent.ownerId = std::stoi(value);

        // talentName
        std::getline(iss, talent.talentName, ',');

        // unlocked
        std::getline(iss, value, ',');
        talent.unlocked = (value == "1" || value == "true");

        currentLevel.talents.push_back(talent);
      }
      else if (type == "Player" || type == "AI")
      {
        LevelState::PlayerInfo player;

        // id
        std::getline(iss, value, ',');
        player.id = std::stoi(value);

        // crystals
        std::getline(iss, value, ',');
        player.crystals = std::stoi(value);

        // wood
        std::getline(iss, value, ',');
        player.wood = std::stoi(value);

        currentLevel.players.push_back(player);
      }
      else
      {
        printf("Unknown entity type in save file: %s\n", type.c_str());
      }
    }
    catch (const std::invalid_argument &ia)
    {
      printf("Invalid argument: Unable to convert string to number in save file line: %s\n", line.c_str());
    }
    catch (const std::exception &e)
    {
      printf("Exception caught while processing save file line: %s\n", line.c_str());
    }
  }

  if (isReadingLevel)
  {
    levelStates[currentLevel.levelName] = currentLevel;
  }

  file.close();
}

void Save::save()
{
  std::ofstream file(filePath, std::ofstream::trunc);
  if (!file.is_open())
  {
    printf("Cannot open save file  %s\n", filePath.c_str());
    return;
  }

  for (const auto &pair : levelStates)
  {
    const LevelState &level = pair.second;
    file << "[" << level.levelName << "]\n";

    for (const auto &unit : level.units)
    {
      file << "Unit," << unit.ownerId << "," << unit.health << "," << unit.type << "," << unit.coords.first << "," << unit.coords.second << "\n";
    }

    for (const auto &castle : level.castles)
    {
      file << "Castle," << castle.ownerId << "," << castle.health << "\n";
    }

    for (const auto &talent : level.talents)
    {
      file << "Talent," << talent.ownerId << talent.talentName << "," << talent.unlocked << "\n";
    }

    for (const auto &player : level.players)
    {
      file << (player.id == 0 ? "Player," : "AI,") << player.id << "," << player.crystals << "," << player.wood << "\n";
    }

    file << "\n";
  }

  file.close();
}
