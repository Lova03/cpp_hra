#include "Worker.h"
#include "utils.h"

Worker::Worker(int x, int y, int width, int height, std::string type, int health, float speed, uint32_t gatherRate, int &wood, int &crystals, int ownerId, float radius, std::vector<std::unique_ptr<Unit>> &allUnits, std::vector<Unit *> &unitsToRemove, std::vector<std::unique_ptr<Wall>> &allWalls, std::vector<std::unique_ptr<Resource>> &allResources, std::vector<Castle *> &allCastles)
    : Unit(x, y, width, height, type, health, speed, ownerId, radius, allUnits, unitsToRemove, allWalls, allResources, allCastles), gatherRate(gatherRate), wood(wood), crystals(crystals) {}

Worker::~Worker() = default;

Worker *Worker::clone() const
{
  return new Worker(*this);
}

void Worker::update()
{
  nextStep();
  handleCollisions();

  if (force.first != 0.0f || force.second != 0.0f)
  {
    applyForce();
  }

  uint32_t now = SDL_GetTicks();
  uint32_t timeSinceLastInteraction = now - lastInteraction;

  if (timeSinceLastInteraction >= gatherRate)
  {
    // Reset the timer
    lastInteraction = now;

    // Create a vector to hold the resources that are in range
    std::vector<Resource *> inRangeResources;
    for (const auto &potentialResource : allResources)
    {
      if (isInRange(*potentialResource))
      {
        inRangeResources.push_back(potentialResource.get());
      }
    }

    if (!inRangeResources.empty())
    {
      // Select a random resource from the inRangeResources vector
      int randomIndex = randomInt(0, inRangeResources.size() - 1);
      gatherResource(*inRangeResources[randomIndex]);
    }
  }
}

void Worker::gatherResource(Resource &resource)
{
  if (resource.getType() == "crystals")
  {
    crystals += randomInt(1, 8);
  }
  if (resource.getType() == "wood")
  {
    wood += randomInt(1, 8);
  }
}