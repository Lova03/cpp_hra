#include "AI.h"
#include "Game.h"
#include "utils.h"
#include "set"
#include <cmath>

AI::AI(int x, int y, int id, std::vector<std::unique_ptr<Unit>> &allUnits, std::vector<Unit *> &unitsToRemove, std::vector<std::unique_ptr<Wall>> &allWalls, std::vector<std::unique_ptr<Resource>> &allResources, std::vector<Castle *> &allCastles)
    : allUnits(allUnits),
      allCastles(allCastles),
      allResources(allResources),
      speedMultiplier(1.0),
      healthMultiplier(1.0),
      spawnRateMultiplier(1.0),
      hasteMultiplier(1.0),
      baseAttackDamage(8),
      baseAttackSpeed(3000),
      gatherRate(8000),
      wood(0),
      crystals(0),
      spawnInterval(15000),
      id(id),
      castle(x, y, 48, 48, id, allUnits, unitsToRemove, allWalls, allResources, allCastles, speedMultiplier, healthMultiplier, spawnRateMultiplier, hasteMultiplier, baseAttackDamage, baseAttackSpeed, gatherRate, wood, crystals, spawnInterval)
{
  lastCastleHP = castle.getHealth();
  talentManager = std::make_unique<TalentManager>(crystals, wood, speedMultiplier, healthMultiplier, spawnRateMultiplier, hasteMultiplier, baseAttackDamage);
}

AI::~AI() = default;

void AI::update()
{
  castle.update();
  decideAction();
}

void AI::render()
{
  castle.render();
}

Castle &AI::getCastle()
{
  return castle;
}

void AI::addUnit(int x, int y, const std::string &type, int health)
{
  castle.spawnUnit(x, y, type, health);
}

void AI::addCrystals(int amount) { crystals += amount; }

void AI::addWood(int amount) { wood += amount; }

TalentManager *AI::getTalentManager()
{
  return talentManager.get();
}

int AI::getCrystals() const { return crystals; }

int AI::getWood() const { return wood; }

int AI::getId() const { return id; }

void AI::decideAction()
{

  std::vector<Unit *> ownedUnits;
  std::vector<Unit *> ownedSoldiers;
  std::vector<Unit *> ownedWorkers;

  int availableSoldiers = 0;

  for (auto &unit : allUnits)
  {
    if (unit->getOwnerId() == id && unit->isAlive())
    {
      ownedUnits.push_back(unit.get());
      if (unit->getType() == "soldier")
      {
        ownedSoldiers.push_back(unit.get());
        if (!unit->isMoving())
          availableSoldiers++;
      }
      else
      {
        ownedWorkers.push_back(unit.get());
      }
    }
  }

  if (castle.getHealth() != lastCastleHP)
  {
    lastCastleHP = castle.getHealth();

    int randomSoldierCount = randomInt(1, 3);
    int sent = 0;

    for (auto &unit : ownedSoldiers)
    {
      if (!unit->isMoving())
      {
        sent++;
        availableSoldiers--;
        unit->moveTo(castle.getDamageFrom().first, castle.getDamageFrom().second);
        if (randomSoldierCount == sent)
          break;
      }
    }
    if (randomSoldierCount != sent)
    {
      for (auto &unit : ownedSoldiers)
      {
        sent++;
        unit->moveTo(castle.getDamageFrom().first, castle.getDamageFrom().second);
        if (randomSoldierCount == sent)
          break;
      }
    }
    printf("AI with ID %d is guarding its castle\n", id);
  }

  int neededUnitsForArmy = randomInt(4, 8);
  int neededUnitsForSnipes = randomInt(4, 8);

  for (auto &unit : ownedWorkers)
  {
    // Find resource for idle workers
    if (!unit->isMoving())
    {

      bool isMining = false;
      for (auto &resource : allResources)
      {
        if (unit->isInRange(*resource))
        {
          isMining = true;
          break;
        }
      }

      if (isMining)
        continue;

      bool picked = false;
      std::set<int> visited;
      int randomResourceIndex = randomInt(0, allResources.size() - 1);

      while (!picked)
      {
        while (visited.find(randomResourceIndex) != visited.end())
        {
          if (visited.size() == allResources.size())
            break;
          randomResourceIndex = randomInt(0, allResources.size() - 1);
        }

        if (visited.size() == allResources.size())
          break;
        picked = !checkAround(*allResources[randomResourceIndex], 1, true);
        if (!picked)
          visited.insert(randomResourceIndex);
      }

      std::pair<int, int> coords = pickRandomTileAround(*allResources[randomResourceIndex]);

      unit->moveTo(coords.first, coords.second);
      printf("AI with ID %d is sending worker to mine at coords x: %d, y: %d\n", id, coords.first, coords.second);
    }
  }

  if (availableSoldiers >= neededUnitsForSnipes)
  {
    availableSoldiers--;
    bool found = false;
    for (auto &resource : allResources)
    {
      found = checkAround(*resource, 2, false, "worker");
      if (found)
      {
        std::pair<int, int> coords = getCoordOfUnitAround(*resource, 2, false, "worker");
        for (auto &unit : ownedSoldiers)
        {
          if (!unit->isMoving())
          {
            unit->moveTo(coords.first, coords.second);
            printf("AI with ID %d is sniping mining worker that is on coords  x: %d, y:%d\n", id, coords.first, coords.second);
            break;
          }
        }
        break;
      }
    }
  }

  if (availableSoldiers >= neededUnitsForArmy)
  {

    bool picked = false;
    std::set<int> visited;
    int randomCastleIndex = randomInt(0, allCastles.size() - 1);
    Castle *targetedCastle = nullptr;

    while (!picked)
    {
      while (visited.find(randomCastleIndex) != visited.end())
      {
        if (visited.size() == allCastles.size())
          break;
        randomCastleIndex = randomInt(0, allCastles.size() - 1);
      }

      if (visited.size() == allCastles.size())
        break;
      picked = (!checkAround(*allCastles[randomCastleIndex], 2, false, "soldier") && allCastles[randomCastleIndex]->getOwnerId() != id && allCastles[randomCastleIndex]->isAlive());
      if (!picked)
        visited.insert(randomCastleIndex);
    }
    if (!picked)
    {
      for (auto &castle : allCastles)
      {
        if (castle->isAlive() && castle->getOwnerId() != id)
        {
          targetedCastle = castle;
          break;
        }
      }
    }
    else
    {
      targetedCastle = allCastles[randomCastleIndex];
    }

    int sentOut = 0;

    for (auto &unit : ownedSoldiers)
    {
      if (!unit->isMoving())
      {
        sentOut++;
        availableSoldiers--;
        std::pair<int, int> coords = pickRandomTileAround(*targetedCastle);
        unit->moveTo(coords.first, coords.second);
        if (sentOut == neededUnitsForArmy)
        {
          printf("AI with ID %d is attacking castle with ID %d with an army of %d units\n", id, targetedCastle->getOwnerId(), neededUnitsForArmy);
          break;
        }
      }
    }
  }

  std::vector<Talent> affordableTalents = talentManager->getAffordableTalents();
  if (affordableTalents.size() > 0)
  {
    printf("AI with ID %d is unlocking talent: %s\n", id, affordableTalents.front().name.c_str());
    getTalentManager()->unlockTalent(affordableTalents.front().name);
  }
}

bool AI::checkAround(const GameObject &target, int tiles, bool friendly, std::string type)
{

  int windowWidth, windowHeight;
  SDL_GetWindowSize(Game::window, &windowWidth, &windowHeight);

  int leftX = std::max(target.getPosition().first - tiles * 16, 0);
  int rightX = std::min(target.getPosition().first + target.getSize().first + tiles * 16, windowWidth);
  int topY = std::max(target.getPosition().second - tiles * 16, 88);
  int bottomY = std::min(target.getPosition().second + target.getSize().second + tiles * 16, windowHeight);

  for (auto &unit : allUnits)
  {
    if (unit->getActualX() >= leftX && (unit->getActualX() + unit->getSize().first) <= rightX &&
        unit->getActualY() >= topY && (unit->getActualY() + unit->getSize().second) <= bottomY)
    {
      if (type == "any")
      {
        if (unit->getOwnerId() == id && friendly && !unit->isMoving())
          return true;
        if (unit->getOwnerId() != id && !friendly && !unit->isMoving())
          return true;
      }

      if (type == "soldier")
      {
        if (unit->getOwnerId() == id && friendly && !unit->isMoving() && unit->getType() == "soldier")
          return true;
        if (unit->getOwnerId() != id && !friendly && !unit->isMoving() && unit->getType() == "soldier")
          return true;
      }

      if (type == "worker")
      {
        if (unit->getOwnerId() == id && friendly && !unit->isMoving() && unit->getType() == "worker")
          return true;
        if (unit->getOwnerId() != id && !friendly && !unit->isMoving() && unit->getType() == "worker")
          return true;
      }
    }
  }

  return false;
}

std::pair<int, int> AI::getCoordOfUnitAround(const GameObject &target, int tiles, bool friendly, std::string type)
{
  int windowWidth, windowHeight;
  SDL_GetWindowSize(Game::window, &windowWidth, &windowHeight);

  int leftX = std::max(target.getPosition().first - tiles * 16, 0);
  int rightX = std::min(target.getPosition().first + target.getSize().first + tiles * 16, windowWidth);
  int topY = std::max(target.getPosition().second - tiles * 16, 88);
  int bottomY = std::min(target.getPosition().second + target.getSize().second + tiles * 16, windowHeight);

  for (auto &unit : allUnits)
  {
    if (unit->getActualX() >= leftX && (unit->getActualX() + unit->getSize().first) <= rightX &&
        unit->getActualY() >= topY && (unit->getActualY() + unit->getSize().second) <= bottomY)
    {
      if (type == "any")
      {
        if (unit->getOwnerId() == id && friendly && !unit->isMoving())
          return unit->getPosition();
        if (unit->getOwnerId() != id && !friendly && !unit->isMoving())
          return unit->getPosition();
      }

      if (type == "soldier")
      {
        if (unit->getOwnerId() == id && friendly && !unit->isMoving() && unit->getType() == "soldier")
          return unit->getPosition();
        if (unit->getOwnerId() != id && !friendly && !unit->isMoving() && unit->getType() == "soldier")
          return unit->getPosition();
      }

      if (type == "worker")
      {
        if (unit->getOwnerId() == id && friendly && !unit->isMoving() && unit->getType() == "worker")
          return unit->getPosition();
        if (unit->getOwnerId() != id && !friendly && !unit->isMoving() && unit->getType() == "worker")
          return unit->getPosition();
      }
    }
  }

  return pickRandomTileAround(target);
}

std::pair<int, int> AI::pickRandomTileAround(const GameObject &target)
{
  std::pair<int, int> position = target.getPosition();
  std::pair<int, int> size = target.getSize();

  std::vector<std::pair<int, int>> tilesAround;

  for (int y = position.second; y < position.second + size.second; y += 16)
  {
    tilesAround.push_back({position.first - 16, y});
    tilesAround.push_back({position.first + size.first, y});
  }

  for (int x = position.first; x < position.first + size.first; x += 16)
  {
    tilesAround.push_back({x, position.second - 16});
    tilesAround.push_back({x, position.second + size.second});
  }

  int index = randomInt(0, tilesAround.size() - 1);

  return tilesAround[index];
}