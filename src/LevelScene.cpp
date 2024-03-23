#include "LevelScene.h"
#include "LevelState.h"
#include "Game.h"
#include <string>
#include <vector>
#include <iostream>
#include <set>
#include <fstream>
#include <functional>
#include <algorithm>
#include "AI.h"
#include "Player.h"
#include "GameObject.h"
#include "TextButton.h"

std::unique_ptr<Map> LevelScene::map = nullptr;

LevelScene::LevelScene(const std::pair<std::string, std::string> &levelData) : name(levelData.first), gameOver(false), playerWon(false)
{
  if (!map)
  {
    map = std::make_unique<Map>();
  }

  SDL_Texture *tilesetTexture = Game::resourceManager.loadTexture("./assets/grass_tileset_16x16.png");

  int windowWidth, windowHeight;
  SDL_GetWindowSize(Game::window, &windowWidth, &windowHeight);
  levelMenu = std::make_unique<Menu>();
  endMenu = std::make_unique<Menu>();
  levelMenu->createBackground(tilesetTexture, windowWidth, windowHeight);

  std::function<void()> backAction = []()
  {
    Game::changeState(GameState::LEVEL_SELECT);
  };

  talentsVisible = false;
  std::function<void()> toggleTalentsVisible = [this]()
  {
    talentsVisible = !talentsVisible;
  };

  std::function<void()> saveProgress = [this]()
  {
    saveCurrentLevel();
  };

  levelMenu->addTextButton(10, 19, 200, 50, "BACK", "./assets/empire.ttf", 24, {255, 255, 255, 255}, {0, 0, 0, 0}, {127, 127, 127, 255}, {255, 255, 255, 255}, {255, 255, 255, 255}, backAction);
  levelMenu->addTextButton(10 + 200 + 10, 19, 300, 50, "SAVE PROGRESS", "./assets/empire.ttf", 24, {255, 255, 255, 255}, {0, 0, 0, 0}, {127, 127, 127, 255}, {255, 255, 255, 255}, {255, 255, 255, 255}, saveProgress);

  levelMenu->addImageButton(windowWidth - 10 - 50, 19, 50, 50, "./assets/talents.png", "./assets/talents_hovered.png", toggleTalentsVisible);

  endMenu->setBackgroundColor(SDL_Color{0, 0, 0, 230});

  std::unique_ptr<Text> textElement = std::make_unique<Text>("Game Over!", "./assets/go3v2.ttf", 92, SDL_Color{255, 255, 255, 255}, 0, 0);

  SDL_Rect textDimensions = textElement->getDimensions();

  int textX = (windowWidth - textDimensions.w) / 2;
  int textY = 80;
  textElement->setPosition(textX, textY);

  endMessage = endMenu->addTextAndGet(std::move(textElement));

  endMenu->addTextButton((windowWidth - 300) / 2, (windowHeight - 50) / 2, 300, 50, "BACK TO LEVELS", "./assets/empire.ttf", 24, {255, 255, 255, 255}, {0, 0, 0, 0}, {127, 127, 127, 255}, {255, 255, 255, 255}, {255, 255, 255, 255}, backAction);

  success = loadLevel(levelData.second);

  state = Game::save.getLevelState(levelData.first);
  updateState();
}

bool LevelScene::loadLevel(const std::string &levelFilePath)
{

  std::set<char> validCharacters{'.', 'W', 'P', 'X', 'C', 'T'};

  std::ifstream levelFileStream(levelFilePath);

  if (!levelFileStream.is_open())
  {
    printf("Failed to open level file. Game will stop running.\n");
    Game::isRunning = false;
    return false;
  }

  std::string line;

  bool aiCastlePresent = false;
  bool playerCastlePresent = false;
  bool isBorderWall = true;

  while (std::getline(levelFileStream, line))
  {

    // Remove carriage return ("\r") if it exists at the end of the line
    if (!line.empty() && line.back() == '\r')
    {
      line.pop_back();
    }

    if (!line.empty() && line.substr(0, 2) != "//")
    {
      mapData.push_back(line);
      if (line.find('X') != std::string::npos)
      {
        aiCastlePresent = true;
      }
      if (line.find('P') != std::string::npos)
      {
        playerCastlePresent = true;
      }

      if (mapData.size() == 1)
      {
        for (const auto &c : line)
        {
          if (c != 'W')
          {
            isBorderWall = false;
            break;
          }
        }
      }
      else
      {
        if (line[0] != 'W' || line[line.size() - 1] != 'W')
        {
          isBorderWall = false;
        }
      }
    }
  }
  levelFileStream.close();

  for (const auto &c : mapData.back())
  {
    if (c != 'W')
    {
      isBorderWall = false;
      break;
    }
  }

  if (!isBorderWall)
  {
    printf("The border of the map is not all wall. Game will stop running.\n");
    Game::isRunning = false;
    return false;
  }

  const int expectedRows = 32;
  const int expectedColumns = 50;
  if (mapData.size() != expectedRows)
  {
    printf("Invalid number of rows in the map file. Expected %d rows, but found %zu rows. Game will stop running.\n", expectedRows, mapData.size());
    Game::isRunning = false;
    return false;
  }
  for (const auto &row : mapData)
  {
    if (row.size() != expectedColumns)
    {
      printf("Invalid number of columns in a row of the map file. Expected %d columns, but found %zu columns. Game will stop running.\n", expectedColumns, row.size());
      Game::isRunning = false;
      return false;
    }
  }

  if (!aiCastlePresent)
  {
    printf("No AI Castle present in the map. Game will stop running.\n");
    Game::isRunning = false;
    return false;
  }
  if (!playerCastlePresent)
  {
    printf("No Player Castle present in the map. Game will stop running.\n");
    Game::isRunning = false;
    return false;
  }

  map->load(mapData);

  int y = 0;

  int aiIdCounter = 1;

  for (const auto &row : mapData)
  {
    for (int x = 0; x < static_cast<int>(row.size()); x++)
    {
      if (validCharacters.find(row[x]) == validCharacters.end())
      {
        printf("Invalid character in map file. Game will stop running.\n");
        Game::isRunning = false;
        return false;
      }

      if (row[x] != '.' && row[x] != 'W')
      {

        int size = (row[x] == 'P' || row[x] == 'X') ? 3 : 2;

        if (!isAreaValid(x, y, size, row[x]))
        {
          printf("Invalid square around %c at position (%d, %d). Game will stop running.\n", row[x], x, y);
          Game::isRunning = false;
          return false;
        }
      }

      if (row[x] != '.')
      {

        if (row[x] == 'W')
        {
          allWalls.push_back(std::make_unique<Wall>(x * 16, 88 + y * 16, 16, 16));
        }

        if ((x > 0 && mapData[y][x - 1] == '.') && (y > 0 && mapData[y - 1][x] == '.'))
        {
          if (row[x] == 'P')
          {
            player = std::make_unique<Player>(x * 16, 88 + y * 16, 0, talentsVisible, allUnits, unitsToRemove, allWalls, allResources, allCastles);
          }

          if (row[x] == 'X')
          {
            ais.push_back(std::make_unique<AI>(x * 16, 88 + y * 16, aiIdCounter, allUnits, unitsToRemove, allWalls, allResources, allCastles));
            aiIdCounter++;
          }

          if (row[x] == 'T')
          {
            allResources.push_back(std::make_unique<Resource>(x * 16, 88 + y * 16, 32, 32, "wood"));
          }

          if (row[x] == 'C')
          {
            allResources.push_back(std::make_unique<Resource>(x * 16, 88 + y * 16, 32, 32, "crystals"));
          }
        }
      }
    }
    y++;
  }
  return true;
}

bool LevelScene::isAreaValid(int topLeftX, int topLeftY, int size, char expectedCharacter)
{

  // top left corner check
  if ((topLeftX > 0 && mapData[topLeftY][topLeftX - 1] == expectedCharacter) || (topLeftY > 0 && mapData[topLeftY - 1][topLeftX] == expectedCharacter))
  {
    return true;
  }

  // space around check
  for (int dx = -1; dx <= size; ++dx)
  {
    for (int dy = -1; dy <= size; ++dy)
    {
      // valid coords
      if (topLeftX + dx < 0 || topLeftX + dx >= static_cast<int>(mapData[0].size()) || topLeftY + dy < 0 || topLeftY + dy >= static_cast<int>(mapData.size()))
      {
        return false;
      }
      // check type or dot
      char c = mapData[topLeftY + dy][topLeftX + dx];

      if ((dx >= 0 && dx < size && dy >= 0 && dy < size && c != expectedCharacter) || // in area
          ((dx == -1 || dx == size || dy == -1 || dy == size) && c != '.'))
      { // outside area
        return false;
      }
    }
  }
  return true;
}

void LevelScene::updateState()
{
  if (!state)
    return;

  // Loop through all talents and add them using the talent manager's enableTalent method
  for (auto &talentInfo : state->talents)
  {
    if (talentInfo.ownerId == 0)
    {
      // If the ownerId is 0, it's a player's talent
      player->getTalentManager()->enableTalent(talentInfo.talentName);
    }
    else
    {
      // Otherwise, it's an AI's talent
      for (auto &ai : ais)
      {
        if (ai->getId() == talentInfo.ownerId)
        {
          ai->getTalentManager()->enableTalent(talentInfo.talentName);
          break;
        }
      }
    }
  }

  // Loop through all units and set them for all players
  for (auto &unitInfo : state->units)
  {
    if (unitInfo.ownerId == 0)
    {
      // If the ownerId is 0, it's a player's unit
      player->addUnit(unitInfo.coords.first, unitInfo.coords.second, unitInfo.type, unitInfo.health);
    }
    else
    {
      // Otherwise, it's an AI's unit
      for (auto &ai : ais)
      {
        if (ai->getId() == unitInfo.ownerId)
        {
          ai->addUnit(unitInfo.coords.first, unitInfo.coords.second, unitInfo.type, unitInfo.health);
          break;
        }
      }
    }
  }

  // Loop through all castles and set their health
  for (auto &castleInfo : state->castles)
  {
    if (castleInfo.ownerId == 0)
    {
      // If the ownerId is 0, it's a player's castle
      player->getCastle().setHealth(castleInfo.health);
      if (castleInfo.health <= 0)
        player->getCastle().die();
    }
    else
    {
      // Otherwise, it's an AI's castle
      for (auto &ai : ais)
      {
        if (ai->getId() == castleInfo.ownerId)
        {
          ai->getCastle().setHealth(castleInfo.health);
          if (castleInfo.health <= 0)
            ai->getCastle().die();
          break;
        }
      }
    }
  }

  // Loop through all players and add crystals and wood
  for (auto &playerInfo : state->players)
  {
    if (playerInfo.id == 0)
    {
      // If the ownerId is 0, it's a player
      player->addCrystals(playerInfo.crystals);
      player->addWood(playerInfo.wood);
    }
    else
    {
      // Otherwise, it's an AI
      for (auto &ai : ais)
      {
        if (ai->getId() == playerInfo.id)
        {
          ai->addCrystals(playerInfo.crystals);
          ai->addWood(playerInfo.wood);
          break;
        }
      }
    }
  }
}

void LevelScene::saveCurrentLevel()
{
  LevelState newState;

  newState.levelName = name;

  for (auto &unit : allUnits)
  {
    LevelState::UnitInfo unitInfo;
    unitInfo.ownerId = unit->getOwnerId();
    unitInfo.health = unit->getHealth();
    unitInfo.type = unit->getType();
    unitInfo.coords = unit->getPosition();
    newState.units.push_back(unitInfo);
  }

  for (auto &castle : allCastles)
  {
    LevelState::CastleInfo castleInfo;
    castleInfo.ownerId = castle->getOwnerId();
    castleInfo.health = castle->getHealth();
    newState.castles.push_back(castleInfo);
  }

  for (auto &ai : ais)
  {
    LevelState::PlayerInfo playerInfo;
    playerInfo.id = ai->getId();
    playerInfo.crystals = ai->getCrystals();
    playerInfo.wood = ai->getWood();
    newState.players.push_back(playerInfo);
  }

  LevelState::PlayerInfo playerInfo;
  playerInfo.id = player->getId();
  playerInfo.crystals = player->getCrystals();
  playerInfo.wood = player->getWood();
  newState.players.push_back(playerInfo);

  Game::save.saveLevelState(newState);
}

void LevelScene::update()
{
  if (!success)
    return;

  if (!gameOver)
  {
    levelMenu->update();

    if (!talentsVisible)
    {
      for (auto &unit : allUnits)
      {
        unit->update();
      }

      if (player)
      {
        player->getCastle().update();
        if (!player->getCastle().isAlive())
        {

          std::unique_ptr<Text> textElement = std::make_unique<Text>("You Lost!", "./assets/go3v2.ttf", 36, SDL_Color{255, 255, 255, 255}, 0, 0);

          SDL_Rect textDimensions = textElement->getDimensions();
          SDL_Rect endMessageDimension = endMessage->getDimensions();

          int windowWidth, windowHeight;
          SDL_GetWindowSize(Game::window, &windowWidth, &windowHeight);

          int textX = (windowWidth - textDimensions.w) / 2;
          int textY = 80 + endMessageDimension.h;
          textElement->setPosition(textX, textY);

          endMenu->addText(std::move(textElement));

          gameOver = true;
        }
      }

      bool allAIsDead = true;
      for (auto &ai : ais)
      {
        ai->update();
        if (ai->getCastle().isAlive())
          allAIsDead = false;
      }
      if (allAIsDead)
      {
        endMessage->setText("Victory!");
        int windowWidth, windowHeight;
        SDL_GetWindowSize(Game::window, &windowWidth, &windowHeight);

        SDL_Rect endMessageDimension = endMessage->getDimensions();
        endMessage->setPosition((windowWidth - endMessageDimension.w) / 2, 80);

        gameOver = true;
      }
    }

    for (auto &unit : unitsToRemove)
    {
      allUnits.erase(std::remove_if(allUnits.begin(), allUnits.end(),
                                    [&unit](const std::unique_ptr<Unit> &u)
                                    { return u.get() == unit; }),
                     allUnits.end());
    }

    if (player)
    {
      auto &selectedUnits = player->getSelectedUnits();
      selectedUnits.erase(
          std::remove_if(selectedUnits.begin(), selectedUnits.end(),
                         [this](Unit *unit)
                         {
                           return std::none_of(allUnits.begin(), allUnits.end(),
                                               [unit](const std::unique_ptr<Unit> &u)
                                               { return u.get() == unit; });
                         }),
          selectedUnits.end());
      player->update();
    }

    unitsToRemove.clear();
  }
  else
  {
    endMenu->update();
  }
}

void LevelScene::render()
{
  if (!success)
    return;

  levelMenu->renderBackground();

  for (auto &wall : allWalls)
  {
    wall->render();
  }

  player->getCastle().render();

  for (auto &resource : allResources)
  {
    resource->render();
  }

  for (auto &ai : ais)
  {
    ai->render();
  }

  for (auto &unit : allUnits)
  {
    unit->render();
  }

  if (player)
  {
    player->render();
  }

  levelMenu->render();

  if (gameOver)
  {
    endMenu->renderBackground();
    endMenu->render();
  }
}

void LevelScene::handleInput(SDL_Event &event)
{
  if (!success)
    return;

  if (player && !gameOver)
  {
    player->handleInput(event);
  }

  switch (event.type)
  {
  case SDL_QUIT:
    Game::isRunning = false;
    break;
  case SDL_MOUSEBUTTONUP:
    if (event.button.button == SDL_BUTTON_LEFT)
    {
      int mouseX = event.button.x;
      int mouseY = event.button.y;

      if (!gameOver)
      {
        levelMenu->handleClick(mouseX, mouseY);
      }
      else
      {
        endMenu->handleClick(mouseX, mouseY);
      }
    }
    break;
  case SDL_KEYDOWN:
    if (event.key.keysym.sym == SDLK_ESCAPE)
      Game::isRunning = false;
  default:
    break;
  }
}