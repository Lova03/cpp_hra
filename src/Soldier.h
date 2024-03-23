#ifndef SOLDIER_H
#define SOLDIER_H

#include "Unit.h"
#include "Wall.h"
#include "Resource.h"
#include <memory>
#include <vector>

/**
 * @class Soldier
 * @brief Represents a soldier unit in the game.
 *
 * The Soldier class inherits from the Unit class and represents a soldier unit in the game.
 * It includes properties and behaviors specific to a soldier, such as attacking units and castles.
 */
class Soldier : public Unit
{
public:
  /**
   * @brief Construct a new Soldier object.
   *
   * @param x The x-coordinate of the soldier's position.
   * @param y The y-coordinate of the soldier's position.
   * @param width The width of the soldier object.
   * @param height The height of the soldier object.
   * @param type The type of the soldier.
   * @param health The health points of the soldier.
   * @param speed The movement speed of the soldier.
   * @param baseAttackDamage The base damage the soldier can inflict.
   * @param attackSpeed The attack speed of the soldier.
   * @param ownerId The id of the owner of the soldier.
   * @param radius The collision radius of the soldier.
   * @param allUnits Reference to all units in the game.
   * @param unitsToRemove Reference to units that need to be removed from the game.
   * @param allWalls Reference to all walls in the game.
   * @param allResources Reference to all resources in the game.
   * @param allCastles Reference to all castles in the game.
   */
  Soldier(int x, int y, int width, int height, std::string type, int health, float speed, int baseAttackDamage, uint32_t attackSpeed, int ownerId, float radius, std::vector<std::unique_ptr<Unit>> &allUnits, std::vector<Unit *> &unitsToRemove, std::vector<std::unique_ptr<Wall>> &allWalls, std::vector<std::unique_ptr<Resource>> &allResources, std::vector<Castle *> &allCastles);

  /**
   * @brief Destroy the Soldier object.
   */
  ~Soldier();

  /**
   * @brief Update the soldier state, includes moving, handling collisions and attacking targets.
   *
   * The soldier will try to attack units first. If no units are in range, it will attack castles in range.
   */
  void update() override;

  /**
   * @brief Creates a new Soldier object that is an exact copy of this Soldier.
   *
   * @return Pointer to the cloned Soldier object.
   */
  Soldier *clone() const override;

  /**
   * @brief Make the soldier attack a unit.
   *
   * The damage inflicted on the unit is random within a range around the soldier's base attack damage.
   *
   * @param target Reference to the unit being attacked.
   */
  void attack(Unit &target);

  /**
   * @brief Make the soldier attack a castle.
   *
   * The damage inflicted on the castle is random within a range around the soldier's base attack damage.
   *
   * @param target Reference to the castle being attacked.
   */
  void attack(Castle &target);

  /**
   * @brief Set the base attack damage of the soldier.
   *
   * @param damage The new base attack damage.
   */
  void setAttackDamage(int damage);

  /**
   * @brief Get the base attack damage of the soldier.
   *
   * @return The base attack damage.
   */
  int getAttackDamage() const;

private:
  int baseAttackDamage;
  uint32_t attackSpeed;
};

#endif
