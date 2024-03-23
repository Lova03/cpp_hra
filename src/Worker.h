#ifndef WORKER_H
#define WORKER_H

#include "Unit.h"
#include "Resource.h"

/**
 * @class Worker
 * @brief Represents a worker unit in the game.
 *
 * The Worker class is a derived class of the Unit class and represents a worker unit in the game.
 * Workers are units that can gather resources and perform various tasks for the player.
 * They have a specific gather rate and can gather resources such as wood and crystals.
 */
class Worker : public Unit
{
public:
  /**
   * @brief Constructor for the Worker class.
   *
   * This constructor creates a worker object with the specified position, width, height, type, health, speed,
   * gather rate, owner ID, radius, and references to the wood and crystals resources.
   *
   * @param x The x-coordinate of the top-left corner of the worker.
   * @param y The y-coordinate of the top-left corner of the worker.
   * @param width The width of the worker.
   * @param height The height of the worker.
   * @param type The type of the worker.
   * @param health The health of the worker.
   * @param speed The speed of the worker.
   * @param gatherRate The rate at which the worker gathers resources.
   * @param wood Reference to the wood resource.
   * @param crystals Reference to the crystals resource.
   * @param ownerId The ID of the owner of the worker.
   * @param radius The radius of the worker.
   * @param allUnits Reference to the vector of all units in the game.
   * @param unitsToRemove Reference to the vector of units to remove from the game.
   * @param allWalls Reference to the vector of all walls in the game.
   * @param allResources Reference to the vector of all resources in the game.
   * @param allCastles Reference to the vector of all castles in the game.
   */
  Worker(int x, int y, int width, int height, std::string type, int health, float speed, uint32_t gatherRate, int &wood, int &crystals, int ownerId, float radius, std::vector<std::unique_ptr<Unit>> &allUnits, std::vector<Unit *> &unitsToRemove, std::vector<std::unique_ptr<Wall>> &allWalls, std::vector<std::unique_ptr<Resource>> &allResources, std::vector<Castle *> &allCastles);

  /**
   * @brief Destructor for the Worker class.
   *
   * This destructor is the default destructor for the Worker class.
   */
  ~Worker();

  /**
   * @brief Update the worker.
   *
   * This function updates the worker's position, handles collisions, applies forces,
   * and gathers resources at the specified gather rate.
   */
  void update() override;

  /**
   * @brief Creates a new Worker object that is an exact copy of this Worker.
   *
   * @return Pointer to the cloned Worker object.
   */
  Worker *clone() const override;

  /**
   * @brief Gather a resource.
   *
   * This function allows the worker to gather a resource based on its type.
   * If the resource is "crystals", the worker adds a random amount of crystals to its owners inventory.
   * If the resource is "wood", the worker adds a random amount of wood to its owners inventory.
   *
   * @param resource The resource to gather.
   */
  void gatherResource(Resource &resource);

private:
  uint32_t gatherRate;
  int &wood;
  int &crystals;
};

#endif