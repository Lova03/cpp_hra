#ifndef RESOURCE_H
#define RESOURCE_H

#include "GameObject.h"
#include <string>

/**
 * @class Resource
 * @brief Represents a resource in the game.
 *
 * The Resource class is a subclass of GameObject and represents a resource object in the game world.
 * It holds the properties and behaviors specific to resources, such as type and texture.
 */
class Resource : public GameObject
{
public:
  /**
   * @brief Constructs a new Resource instance.
   *
   * Sets up the resource with the given properties and sets its texture based on its type.
   *
   * @param x The x-coordinate of the resource.
   * @param y The y-coordinate of the resource.
   * @param width The width of the resource.
   * @param height The height of the resource.
   * @param type The type of the resource, can be "crystals" or "wood".
   */
  Resource(int x, int y, int width, int height, std::string type);

  /**
   * @brief Default destructor for the Resource class.
   */
  ~Resource();

  /**
   * @brief Returns the type of the resource.
   *
   * @return The type of the resource as a string.
   */
  std::string getType() const;

  /**
   * @brief Sets the type of the resource.
   *
   * @param newType The new type for the resource, can be "crystals" or "wood".
   */
  void setType(std::string newType);

private:
  std::string type;
};

#endif