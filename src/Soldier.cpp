#include "Soldier.h"
#include "utils.h"
#include <cmath>
#include <algorithm>

Soldier::Soldier(int x, int y, int width, int height, std::string type, int health, float speed, int baseAttackDamage, uint32_t attackSpeed, int ownerId, float radius, std::vector<std::unique_ptr<Unit>> &allUnits, std::vector<Unit *> &unitsToRemove, std::vector<std::unique_ptr<Wall>> &allWalls, std::vector<std::unique_ptr<Resource>> &allResources, std::vector<Castle *> &allCastles)
    : Unit(x, y, width, height, type, health, speed, ownerId, radius, allUnits, unitsToRemove, allWalls, allResources, allCastles), baseAttackDamage(baseAttackDamage), attackSpeed(attackSpeed) {}

Soldier::~Soldier() = default;

void Soldier::update()
{
  nextStep();
  handleCollisions();

  if (force.first != 0.0f || force.second != 0.0f)
  {
    applyForce();
  }

  uint32_t now = SDL_GetTicks();
  uint32_t timeSinceLastInteraction = now - lastInteraction;

  if (timeSinceLastInteraction >= attackSpeed)
  {
    // Reset the timer
    lastInteraction = now;

    bool didAttack = false;

    // Find a target and attack
    for (const auto &potentialTarget : allUnits)
    {
      if (potentialTarget->getOwnerId() != ownerId && isInRange(*potentialTarget))
      {
        attack(*potentialTarget);
        didAttack = true;
        break; // exit the loop once we've attacked
      }
    }

    // If no units were in range, check for castles
    for (const auto &potentialTarget : allCastles)
    {
      if (didAttack)
        break;
      if (potentialTarget->getOwnerId() != ownerId && isInRange(*potentialTarget))
      {
        attack(*potentialTarget);
        break; // exit the loop once we've attacked
      }
    }
  }
}

Soldier *Soldier::clone() const
{
  return new Soldier(*this);
}

void Soldier::attack(Unit &target)
{
  target.takeDamage(randomInt(std::max(1, baseAttackDamage - 2), baseAttackDamage + 2));
}

void Soldier::attack(Castle &target)
{
  target.takeDamage(randomInt(std::max(1, baseAttackDamage - 2), baseAttackDamage + 2), getPosition());
}

void Soldier::setAttackDamage(int damage)
{
  baseAttackDamage = damage;
}

int Soldier::getAttackDamage() const
{
  return baseAttackDamage;
}
