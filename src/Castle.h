#ifndef CASTLE_H
#define CASTLE_H

#include "GameObject.h"
#include "Map.h"
#include "Wall.h"
#include "Resource.h"
#include "Castle.h"
#include "Unit.h"
#include <string>
#include <utility>
#include <vector>
#include <memory>

class Unit;

/**
 * @class Castle
 * @brief Game Castle Object
 *
 * The Castle class represents a castle object in the game. It serves as the base for a player (human & ai).
 * Castles have health, can spawn units, take damage, and perform various actions in the game.
 * This class inherits from the GameObject class.
 */
class Castle : public GameObject
{
public:
  /**
   * @brief Construct a new Castle object
   *
   * @param x x-coordinate of the castle.
   * @param y y-coordinate of the castle.
   * @param width Width of the castle.
   * @param height Height of the castle.
   * @param ownerId ID of the castle owner.
   * @param allUnits Reference to the vector holding all the units.
   * @param unitsToRemove Reference to the vector holding the units to be removed.
   * @param allWalls Reference to the vector holding all the walls.
   * @param allResources Reference to the vector holding all the resources.
   * @param allCastles Reference to the vector holding all the castles.
   * @param speedMultiplier Multiplier for speed.
   * @param healthMultiplier Multiplier for health.
   * @param spawnRateMultiplier Multiplier for spawn rate.
   * @param hasteMultiplier Multiplier for haste.
   * @param baseAttackDamage Base attack damage.
   * @param baseAttackSpeed Base attack speed.
   * @param gatherRate Gathering rate.
   * @param wood Amount of wood.
   * @param crystals Amount of crystals.
   * @param spawnInterval Spawn interval.
   */
  Castle(int x, int y, int width, int height, int ownerId, std::vector<std::unique_ptr<Unit>> &allUnits, std::vector<Unit *> &unitsToRemove, std::vector<std::unique_ptr<Wall>> &allWalls, std::vector<std::unique_ptr<Resource>> &allResources, std::vector<Castle *> &allCastles, float &speedMultiplier, float &healthMultiplier, float &spawnRateMultiplier, float &hasteMultiplier, int &baseAttackDamage, uint32_t &baseAttackSpeed, uint32_t &gatherRate, int &wood, int &crystals, uint32_t &spawnInterval);

  /**
   * @brief Default destructor of the Castle object
   *
   */
  virtual ~Castle() = default;

  /**
   * @brief Updates the state of the Castle.
   * Periodically spawns units if the Castle is alive.
   */
  void update() override;

  /**
   * @brief Renders the Castle and its health bar on the screen.
   */
  void render() override;

  /**
   * @brief Spawns a unit of a specific type near the Castle.
   * @param type The type of the unit ("soldier" or "worker").
   */
  void spawnUnit(const std::string &type);

  /**
   * @brief Spawns a unit of a specific type at a specified location with a specified health.
   * Method used by Save object to load in saved Units
   * @param x The x coordinate of the new unit's position.
   * @param y The y coordinate of the new unit's position.
   * @param type The type of the unit ("soldier" or "worker").
   * @param health The health of the new unit.
   */
  void spawnUnit(int x, int y, const std::string &type, int health);

  /**
   * @brief Spawns a random unit type near the Castle.
   */
  void spawnUnit();

  /**
   * @brief Decreases the Castle's health when it takes damage and
   * saves position from which it's being attacked from.
   * AI objects then uses the position
   * @param damage The amount of damage the Castle takes.
   * @param attackedPos The position from which the Castle is attacked.
   */
  void takeDamage(int damage, std::pair<int, int> attackedPos);

  /**
   * @brief Sets the health of the Castle.
   * @param health The new health value.
   */
  void setHealth(int health);

  /**
   * @brief Gets the current health of the Castle.
   * @return The current health of the Castle.
   */
  int getHealth() const;

  /**
   * @brief Sets the owner ID of the Castle.
   * @param ownerId The new owner ID.
   */
  void setOwnerId(int ownerId);

  /**
   * @brief Gets the owner ID of the Castle.
   * @return The owner ID of the Castle.
   */
  int getOwnerId() const;

  /**
   * @brief Gets the position from which the Castle last took damage.
   * @return A pair representing the (x, y) position of the last attack.
   */
  std::pair<int, int> getDamageFrom() const;

  /**
   * @brief Checks if the Castle is alive (has positive health).
   * @return True if the Castle is alive, false otherwise.
   */
  bool isAlive() const;

  /**
   * @brief Handles the Castle's destruction and takes appropriate actions.
   */
  void die();

private:
  int maxHealth;
  int health;
  int ownerId;

  std::vector<std::unique_ptr<Unit>> &allUnits;
  std::vector<Unit *> &unitsToRemove;
  std::vector<std::unique_ptr<Wall>> &allWalls;
  std::vector<std::unique_ptr<Resource>> &allResources;
  std::vector<Castle *> &allCastles;

  float &speedMultiplier;
  float &healthMultiplier;
  float &spawnRateMultiplier;
  float &hasteMultiplier;
  int &baseAttackDamage;

  uint32_t &baseAttackSpeed;
  uint32_t &gatherRate;
  int &wood;
  int &crystals;

  uint32_t &spawnInterval;
  uint32_t lastSpawnTime;

  std::pair<int, int> damageFrom;
};

#endif
