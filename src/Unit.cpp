#include "Game.h"
#include "Unit.h"
#include "Map.h"
#include "utils.h"
#include <cmath>
#include <utility>

Unit::Unit(int x, int y, int width, int height, std::string type, int health, float speed, int ownerId, float radius, std::vector<std::unique_ptr<Unit>> &allUnits, std::vector<Unit *> &unitsToRemove, std::vector<std::unique_ptr<Wall>> &allWalls, std::vector<std::unique_ptr<Resource>> &allResources, std::vector<Castle *> &allCastles)
    : GameObject(x, y, width, height),
      type(type),
      force(0.0f, 0.0f),
      health(health),
      maxHealth(health),
      speed(speed),
      ownerId(ownerId),
      radius(radius),
      allUnits(allUnits),
      unitsToRemove(unitsToRemove),
      allWalls(allWalls),
      allResources(allResources),
      allCastles(allCastles),
      lastInteraction(0)
{
  actualX = x;
  actualY = y;
}

void Unit::render()
{
  if (texture != nullptr)
  {
    SDL_RenderCopy(Game::renderer, texture, NULL, &objectRect);
  }

  // Create a rectangle for the total health (red)
  SDL_Rect healthBarRect;
  healthBarRect.x = objectRect.x;
  healthBarRect.y = objectRect.y - 6; // Position it 6px above the unit
  healthBarRect.w = 16;
  healthBarRect.h = 4;

  // Render the total health bar (red)
  SDL_SetRenderDrawColor(Game::renderer, 255, 0, 0, 255);
  SDL_RenderFillRect(Game::renderer, &healthBarRect);

  // Create a rectangle for the current health (green)
  SDL_Rect currentHealthRect;
  currentHealthRect.x = objectRect.x;
  currentHealthRect.y = objectRect.y - 6;                      // Position it 6px above the unit
  currentHealthRect.w = (int)((float)health / maxHealth * 16); // Scale it according to the current health percentage
  currentHealthRect.h = 4;

  // Render the current health bar (green)
  SDL_SetRenderDrawColor(Game::renderer, 0, 255, 0, 255);
  SDL_RenderFillRect(Game::renderer, &currentHealthRect);
}

void Unit::moveTo(int targetX, int targetY)
{
  calculatePath(targetX, targetY);
}

void Unit::handleCollisions()
{

  for (auto &unit : allUnits)
  {
    // Skip self
    if (this == unit.get())
      continue;

    // If units are intersecting
    if (checkCollision(*unit))
    {
      if (this < unit.get())
      {
        separate(*unit);
      }
    }
  }
}

void Unit::separate(Unit &other)
{
  // Separate units based on their centers and their size
  auto pos1 = getPosition();
  auto pos2 = other.getPosition();
  float dx = pos2.first - pos1.first;
  float dy = pos2.second - pos1.second;
  float distance = std::sqrt(dx * dx + dy * dy);

  float size1 = getSize().first / 2.0;
  float size2 = other.getSize().first / 2.0;

  float overlap = (size1 + size2) - distance;

  // Normalize
  if (distance > 0)
  {
    dx /= distance;
    dy /= distance;
  }

  // Add some randomness to the direction of the force for each unit
  int randomAngle1 = randomInt(-30, 30);                    // This will give us a random angle between -30 and 30 degrees for this unit
  int randomAngle2 = randomInt(-30, 30);                    // This will give us a random angle between -30 and 30 degrees for the other unit
  float randomAngleRadians1 = randomAngle1 * M_PI / 180.0f; // Convert to radians
  float randomAngleRadians2 = randomAngle2 * M_PI / 180.0f; // Convert to radians
  float newDx1 = dx * cos(randomAngleRadians1) - dy * sin(randomAngleRadians1);
  float newDy1 = dx * sin(randomAngleRadians1) + dy * cos(randomAngleRadians1);
  float newDx2 = dx * cos(randomAngleRadians2) - dy * sin(randomAngleRadians2);
  float newDy2 = dx * sin(randomAngleRadians2) + dy * cos(randomAngleRadians2);
  dx = newDx1;
  dy = newDy1;

  // Apply a unique force to each unit
  float randomFactor1 = randomInt(50, 300) / 100.0f;
  float randomFactor2 = randomInt(50, 300) / 100.0f;

  force.first -= overlap * randomFactor1 * dx;
  force.second -= overlap * randomFactor1 * dy;
  dx = newDx2;
  dy = newDy2;

  other.force.first += overlap * randomFactor2 * dx;
  other.force.second += overlap * randomFactor2 * dy;

  float stationaryResistance = 0.8;

  if (isMoving() && !other.isMoving())
  {
    force.first = 0;
    force.second = 0;
    if (randomFactor2 > 2)
    {
      other.force.first *= stationaryResistance;
      other.force.second *= stationaryResistance;
    }
  }

  if (other.isMoving() && !isMoving())
  {
    other.force.first = 0;
    other.force.second = 0;
    if (randomFactor1 > 2)
    {
      force.first *= stationaryResistance;
      force.second *= stationaryResistance;
    }
  }
}

void Unit::stopMovement()
{
  // Clear path
  path.clear();
}

void Unit::calculatePath(int targetX, int targetY)
{
  // Convert pixel coordinates to grid indexes for path calculation
  int gridStartX = actualX / 16;
  int gridStartY = (actualY - 88) / 16;
  int gridTargetX = targetX / 16;
  int gridTargetY = (targetY - 88) / 16;

  if (gridStartX == gridTargetX && gridStartY == gridTargetY)
  {
    return;
  }

  // Get the path from the A* algorithm
  std::list<std::pair<int, int>> gridPath = LevelScene::getMap().calculatePath(std::make_pair(gridStartX, gridStartY), std::make_pair(gridTargetX, gridTargetY));

  // Convert grid indexes to pixel coordinates for movement
  path.clear();
  for (const auto &cell : gridPath)
  {
    int pixelX = cell.first * 16;
    int pixelY = cell.second * 16 + 88; // account for offset of game area
    path.push_back(std::make_pair(pixelX, pixelY));
  }
  if (!path.empty())
  {
    path.pop_front();
  }
}

void Unit::nextStep()
{
  // If unit is under influence of a force, don't execute the path following behavior
  if (force.first != 0.0f || force.second != 0.0f)
    return;

  if (!path.empty())
  {
    auto next = path.front();

    float nextX = next.first;
    float nextY = next.second;

    // Calculate direction
    float dx = nextX - actualX;
    float dy = nextY - actualY;
    float magnitude = std::sqrt(dx * dx + dy * dy);
    dx /= magnitude;
    dy /= magnitude;

    // Apply force to the calculated movement direction
    dx += force.first;
    dy += force.second;

    // Move the unit
    actualX += dx * speed;
    actualY += dy * speed;

    // Update objectRect for rendering
    objectRect.x = (int)actualX;
    objectRect.y = (int)actualY;

    // Check if we reached the target tile
    if (std::abs(actualX - nextX) < speed && std::abs(actualY - nextY) < speed)
    {
      path.pop_front();
      if (path.empty())
      {
        if (objectRect.x % 16 != 0)
        {
          objectRect.x += 16 - (objectRect.x % 16);
          actualX += speed;
        }
        if ((objectRect.y - 88) % 16 != 0)
        {
          objectRect.y += 16 - ((objectRect.y - 88) % 16);
          actualY += speed;
        }
      }
    }
  }
}

std::pair<float, float> Unit::getNextTarget() const
{
  if (!path.empty())
  {
    return path.front();
  }
  else
  {
    return getPosition();
  }
}

void Unit::takeDamage(int damage)
{
  health -= damage;
  if (health <= 0)
  {
    die();
  }
}

bool Unit::isMoving() const
{
  return !path.empty();
}

bool Unit::isAlive() const
{
  return health > 0;
};

void Unit::invertForce()
{
  force.first = -force.first;
  force.second = -force.second;
}

void Unit::applyForce()
{
  // create a "virtual unit" that represents the next position
  Unit *virtualUnit = this->clone();
  virtualUnit->actualX += force.first * 0.05;
  virtualUnit->actualY += force.second * 0.05;
  bool willCollideWithObject = virtualUnit->checkCollisions();

  if (!willCollideWithObject)
  {
    actualX = virtualUnit->actualX;
    actualY = virtualUnit->actualY;

    objectRect.x = (int)actualX;
    objectRect.y = (int)actualY;

    force.first -= force.first * 0.05;
    force.second -= force.second * 0.05;
  }
  else
  {
    // invert force direction if collision would occur
    force.first = -force.first;
    force.second = -force.second;

    actualX += force.first * 0.05;
    actualY += force.second * 0.05;

    objectRect.x = (int)actualX;
    objectRect.y = (int)actualY;

    force.first -= force.first * 0.05;
    force.second -= force.second * 0.05;
  }

  if (std::abs(force.first) < 0.1 && std::abs(force.second) < 0.1)
  {
    force.first = 0.0f;
    force.second = 0.0f;
  }
  delete virtualUnit;
}

bool Unit::checkCollisions()
{
  for (auto &wall : allWalls)
  {
    if (checkCollision(*wall))
    {
      return true;
    }
  }

  for (auto &castle : allCastles)
  {
    if (checkCollision(*castle))
    {
      return true;
    }
  }

  for (auto &resource : allResources)
  {
    if (checkCollision(*resource))
    {
      return true;
    }
  }

  return false;
}

bool Unit::isInRange(const GameObject &target) const
{
  auto targetPos = target.getPosition();
  auto targetSize = target.getSize();

  auto thisPos = getPosition();
  auto thisSize = getSize();
  thisPos.first += thisSize.first / 2;
  thisPos.second += thisSize.second / 2;

  // Find the closest point on the target rectangle to the center of this unit
  float closestX, closestY;

  if (thisPos.first < targetPos.first)
    closestX = targetPos.first;
  else if (thisPos.first > targetPos.first + targetSize.first)
    closestX = targetPos.first + targetSize.first;
  else
    closestX = thisPos.first;

  if (thisPos.second < targetPos.second)
    closestY = targetPos.second;
  else if (thisPos.second > targetPos.second + targetSize.second)
    closestY = targetPos.second + targetSize.second;
  else
    closestY = thisPos.second;

  float dx = thisPos.first - closestX;
  float dy = thisPos.second - closestY;
  float distance = std::sqrt(dx * dx + dy * dy);

  return distance <= radius * 16;
}

void Unit::die()
{
  unitsToRemove.push_back(this);
}

int Unit::getHealth() const { return health; };
void Unit::setHealth(int newHealth) { health = newHealth; };
float Unit::getSpeed() const { return speed; };
void Unit::setSpeed(float newSpeed) { speed = newSpeed; };
int Unit::getOwnerId() const { return ownerId; };
void Unit::setOwnerId(int newOwnerId) { ownerId = newOwnerId; };
float Unit::getRadius() const { return radius; };
void Unit::setRadius(float newRadius) { radius = newRadius; };
float Unit::getActualX() const { return actualX; };
void Unit::setActualX(float x) { actualX = x; };
float Unit::getActualY() const { return actualY; };
void Unit::setActualY(float y) { actualY = y; };
std::string Unit::getType() const { return type; };
void Unit::setType(std::string newType) { type = newType; };
std::pair<float, float> Unit::getForce() const { return force; };
void Unit::setForce(std::pair<float, float> f) { force = f; };