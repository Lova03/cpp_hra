#include "Game.h"
#include "Soldier.h"
#include "Worker.h"
#include "utils.h"

Castle::Castle(int x, int y, int width, int height, int ownerId, std::vector<std::unique_ptr<Unit>> &allUnits, std::vector<Unit *> &unitsToRemove, std::vector<std::unique_ptr<Wall>> &allWalls, std::vector<std::unique_ptr<Resource>> &allResources, std::vector<Castle *> &allCastles, float &speedMultiplier, float &healthMultiplier, float &spawnRateMultiplier, float &hasteMultiplier, int &baseAttackDamage, uint32_t &baseAttackSpeed, uint32_t &gatherRate, int &wood, int &crystals, uint32_t &spawnInterval)
    : GameObject(x, y, width, height),
      maxHealth(250),
      health(250),
      ownerId(ownerId),
      allUnits(allUnits),
      unitsToRemove(unitsToRemove),
      allWalls(allWalls),
      allResources(allResources),
      allCastles(allCastles),
      speedMultiplier(speedMultiplier),
      healthMultiplier(healthMultiplier),
      spawnRateMultiplier(spawnRateMultiplier),
      hasteMultiplier(hasteMultiplier),
      baseAttackDamage(baseAttackDamage),
      baseAttackSpeed(baseAttackSpeed),
      gatherRate(gatherRate),
      wood(wood),
      crystals(crystals),
      spawnInterval(spawnInterval),
      lastSpawnTime(SDL_GetTicks()),
      damageFrom({-1, -1})
{
  setTexture(getCastleTexturePath(ownerId));

  allCastles.push_back(this);

  spawnUnit();
}

void Castle::update()
{
  uint32_t currentTime = SDL_GetTicks();
  if (currentTime - lastSpawnTime >= (spawnInterval * spawnRateMultiplier))
  { // 10000 ms = 10 s

    if (isAlive())
    {
      spawnUnit();
    }
    lastSpawnTime = currentTime;
  }
}

void Castle::render()
{
  if (texture != nullptr)
  {
    SDL_RenderCopy(Game::renderer, texture, NULL, &objectRect);
  }

  // Create a rectangle for the total health (red)
  SDL_Rect healthBarRect;
  healthBarRect.x = objectRect.x;
  healthBarRect.y = objectRect.y - 6; // Position it 6px above the unit
  healthBarRect.w = 48;
  healthBarRect.h = 4;

  // Render the total health bar (red)
  SDL_SetRenderDrawColor(Game::renderer, 255, 0, 0, 255);
  SDL_RenderFillRect(Game::renderer, &healthBarRect);

  // Create a rectangle for the current health (green)
  SDL_Rect currentHealthRect;
  currentHealthRect.x = objectRect.x;
  currentHealthRect.y = objectRect.y - 6;                      // Position it 6px above the unit
  currentHealthRect.w = (int)((float)health / maxHealth * 48); // Scale it according to the current health percentage
  currentHealthRect.h = 4;

  // Render the current health bar (green)
  if (isAlive())
  {
    SDL_SetRenderDrawColor(Game::renderer, 0, 255, 0, 255);
    SDL_RenderFillRect(Game::renderer, &currentHealthRect);
  }
}

void Castle::spawnUnit(int x, int y, const std::string &type, int health)
{

  if (type == "soldier")
  {
    auto soldier = std::make_unique<Soldier>(x, y, 16, 16, "soldier", 60 * healthMultiplier, 0.14 * speedMultiplier, baseAttackDamage, baseAttackSpeed * hasteMultiplier, ownerId, 2.0, allUnits, unitsToRemove, allWalls, allResources, allCastles);
    soldier->setTexture(getSoldierTexturePath(ownerId).first);
    soldier->setHealth(health);
    allUnits.push_back(std::move(soldier));
  }
  if (type == "worker")
  {
    auto worker = std::make_unique<Worker>(x, y, 16, 16, "worker", 40 * healthMultiplier, 0.22 * speedMultiplier, gatherRate * hasteMultiplier, wood, crystals, ownerId, 1.0, allUnits, unitsToRemove, allWalls, allResources, allCastles);
    worker->setTexture(getWorkerTexturePath(ownerId).first);
    worker->setHealth(health);
    allUnits.push_back(std::move(worker));
  }
}

void Castle::spawnUnit(const std::string &type)
{

  if (type == "soldier")
  {
    auto soldier = std::make_unique<Soldier>((objectRect.x + (objectRect.w / 2)) - 8, objectRect.y + objectRect.h - 16, 16, 16, "soldier", 60 * healthMultiplier, 0.14 * speedMultiplier, baseAttackDamage, baseAttackSpeed * hasteMultiplier, ownerId, 2.0, allUnits, unitsToRemove, allWalls, allResources, allCastles);
    soldier->setTexture(getSoldierTexturePath(ownerId).first);
    soldier->moveTo((objectRect.x + (objectRect.w / 2)) - 8, objectRect.y + objectRect.h);
    allUnits.push_back(std::move(soldier));
  }
  if (type == "worker")
  {
    auto worker = std::make_unique<Worker>((objectRect.x + (objectRect.w / 2)) - 8, objectRect.y + objectRect.h - 16, 16, 16, "worker", 40 * healthMultiplier, 0.22 * speedMultiplier, gatherRate * hasteMultiplier, wood, crystals, ownerId, 1.0, allUnits, unitsToRemove, allWalls, allResources, allCastles);
    worker->setTexture(getWorkerTexturePath(ownerId).first);
    worker->moveTo((objectRect.x + (objectRect.w / 2)) - 8, objectRect.y + objectRect.h);
    allUnits.push_back(std::move(worker));
  }
}

void Castle::spawnUnit()
{
  int randNum = randomInt(0, 100);
  if (randNum < 70)
  {
    spawnUnit("soldier");
  }
  else
  {
    spawnUnit("worker");
  }
}

void Castle::takeDamage(int damage, std::pair<int, int> attackedPos)
{
  damageFrom = attackedPos;
  health -= damage;
  if (health <= 0)
  {
    health = 0;
    die();
  }
}

void Castle::die()
{
  for (auto &unit : allUnits)
  {
    if (unit->getOwnerId() == ownerId)
    {
      unit->setHealth(0);
      unit->die();
    }
  }
}

void Castle::setHealth(int health) { this->health = health; }
int Castle::getHealth() const { return health; }

void Castle::setOwnerId(int ownerId) { this->ownerId = ownerId; }
int Castle::getOwnerId() const { return ownerId; }

std::pair<int, int> Castle::getDamageFrom() const
{
  return damageFrom;
}

bool Castle::isAlive() const
{
  return health > 0;
}