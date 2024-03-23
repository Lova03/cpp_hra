#ifndef UNIT_H
#define UNIT_H

#include "GameObject.h"
#include "Map.h"
#include "Wall.h"
#include "Resource.h"
#include "Castle.h"
#include <list>
#include <vector>
#include <memory>
#include <utility>

/**
 * @class Unit
 * @brief Represents a game unit in the game world.
 *
 * The Unit class is a base class for game units in the game world. It inherits from the GameObject class
 * and provides functionality for units' movement, collision detection, health management, and interaction with other game objects.
 * Derived classes can extend this class to implement specific types of units with additional behavior and properties.
 */
class Unit : public GameObject
{
public:
  /**
   * @brief Constructor for Unit. Initializes a new instance of Unit with specified parameters.
   * @param x The x-coordinate of the unit's starting location.
   * @param y The y-coordinate of the unit's starting location.
   * @param width The width of the unit.
   * @param height The height of the unit.
   * @param type The type of the unit.
   * @param health The initial health of the unit.
   * @param speed The speed of the unit.
   * @param ownerId The ID of the owner of the unit.
   * @param radius The radius within which the unit can interact with other game objects.
   * @param allUnits A reference to a vector containing all units in the game.
   * @param unitsToRemove A reference to a vector containing all units that need to be removed.
   * @param allWalls A reference to a vector containing all walls in the game.
   * @param allResources A reference to a vector containing all resources in the game.
   * @param allCastles A reference to a vector containing all castles in the game.
   */
  Unit(int x, int y, int width, int height, std::string type, int health, float speed, int ownerId, float radius, std::vector<std::unique_ptr<Unit>> &allUnits, std::vector<Unit *> &unitsToRemove, std::vector<std::unique_ptr<Wall>> &allWalls, std::vector<std::unique_ptr<Resource>> &allResources, std::vector<Castle *> &allCastles);

  /**
   * @brief Virtual destructor for the Unit class.
   */
  virtual ~Unit() = default;

  /**
   * @brief Renders the Unit on the screen.
   *
   * This method takes care of rendering the Unit object and its health bar on the screen.
   */
  void render() override;

  /**
   * @brief Pure virtual clone method.
   *
   * Derived classes must provide an implementation to clone unit.
   */
  virtual Unit *clone() const = 0;

  /**
   * @brief Initiates movement of the Unit towards the given target coordinates.
   *
   * @param targetX The x-coordinate of the target location in pixels.
   * @param targetY The y-coordinate of the target location in pixels.
   */
  void moveTo(int targetX, int targetY);

  /**
   * @brief Applies damage to the Unit and handles its death if health drops below or equal to zero.
   *
   * @param damage The amount of damage to apply to the Unit.
   */
  void takeDamage(int damage);

  /**
   * @brief Handles collisions between this Unit and other Units.
   *
   * This method is responsible for detecting and resolving collisions with other Units.
   */
  void handleCollisions();

  /**
   * @brief Checks if there are any collisions.
   *
   * @return true if there are any collisions, false otherwise.
   */
  bool checkCollisions();

  /**
   * @brief Checks if the given target is within the range of the Unit.
   *
   * @param target Reference to the GameObject to check the range against.
   * @return true if the target is within range, false otherwise.
   */
  bool isInRange(const GameObject &target) const;

  /**
   * @brief Separates this Unit from another given Unit.
   *
   * @param other Reference to the other Unit with which separation should occur.
   */
  void separate(Unit &other);

  /**
   * @brief Inverts the direction of the force.
   */
  void invertForce();

  /**
   * @brief Applies the force to the Unit.
   */
  void applyForce();

  /**
   * @brief Returns the next target location on the unit's current path.
   *
   * @return The next target location on the path.
   */
  std::pair<float, float> getNextTarget() const;

  /**
   * @brief Stops the movement of the Unit.
   */
  void stopMovement();

  /**
   * @brief Checks if the Unit is alive.
   *
   * @return true if the Unit is alive, false otherwise.
   */
  bool isAlive() const;

  /**
   * @brief Checks if the Unit is currently moving.
   *
   * @return true if the Unit is moving, false otherwise.
   */
  bool isMoving() const;

  /**
   * @brief Returns the current health of the Unit.
   *
   * @return The current health of the Unit.
   */
  int getHealth() const;

  /**
   * @brief Sets the health of the Unit.
   *
   * @param newHealth The new health value for the Unit.
   */
  void setHealth(int newHealth);

  /**
   * @brief Returns the current speed of the Unit.
   *
   * @return The current speed of the Unit.
   */
  float getSpeed() const;

  /**
   * @brief Sets the speed of the Unit.
   *
   * @param newSpeed The new speed value for the Unit.
   */
  void setSpeed(float newSpeed);

  /**
   * @brief Returns the owner ID of the Unit.
   *
   * @return The owner ID of the Unit.
   */
  int getOwnerId() const;

  /**
   * @brief Sets the owner ID of the Unit.
   *
   * @param newOwnerId The new owner ID for the Unit.
   */
  void setOwnerId(int newOwnerId);

  /**
   * @brief Returns the interaction radius of the Unit.
   *
   * @return The interaction radius of the Unit.
   */
  float getRadius() const;

  /**
   * @brief Sets the interaction radius of the Unit.
   *
   * @param newRadius The new radius for the Unit.
   */
  void setRadius(float newRadius);

  /**
   * @brief Returns the actual x-coordinate of the Unit.
   *
   * @return The actual x-coordinate of the Unit.
   */
  float getActualX() const;

  /**
   * @brief Sets the actual x-coordinate of the Unit.
   *
   * @param x The new x-coordinate for the Unit.
   */
  void setActualX(float x);

  /**
   * @brief Returns the actual y-coordinate of the Unit.
   *
   * @return The actual y-coordinate of the Unit.
   */
  float getActualY() const;

  /**
   * @brief Sets the actual y-coordinate of the Unit.
   *
   * @param y The new y-coordinate for the Unit.
   */
  void setActualY(float y);

  /**
   * @brief Returns the type of the Unit.
   *
   * @return The type of the Unit.
   */
  std::string getType() const;

  /**
   * @brief Sets the type of the Unit.
   *
   * @param newType The new type for the Unit.
   */
  void setType(std::string newType);

  /**
   * @brief Returns the current force acting on the Unit.
   *
   * @return A pair of floats representing the current force acting on the Unit.
   */
  std::pair<float, float> getForce() const;

  /**
   * @brief Sets the force to be applied on the Unit.
   *
   * @param f A pair of floats representing the force to be applied on the Unit.
   */
  void setForce(std::pair<float, float> f);

  /**
   * @brief Handles the death of the Unit.
   */
  void die();

protected:
  /**
   * @brief Calculates the path to the target location.
   *
   * @param targetX The x-coordinate of the target location in pixels.
   * @param targetY The y-coordinate of the target location in pixels.
   */
  void calculatePath(int targetX, int targetY);

  /**
   * @brief Executes the next movement step for the Unit.
   */
  void nextStep();

  std::string type;

  std::pair<float, float> force;

  int health;
  int maxHealth;
  float speed;
  int ownerId;
  float radius;

  float actualX, actualY;
  std::list<std::pair<int, int>> path;
  std::vector<std::unique_ptr<Unit>> &allUnits;
  std::vector<Unit *> &unitsToRemove;
  std::vector<std::unique_ptr<Wall>> &allWalls;
  std::vector<std::unique_ptr<Resource>> &allResources;
  std::vector<Castle *> &allCastles;
  uint32_t lastInteraction;
};

#endif
