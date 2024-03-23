#ifndef WALL_H
#define WALL_H

#include "GameObject.h"

/**
 * @class Wall
 * @brief Represents a wall object in the game world.
 *
 * The Wall class represents a wall object in the game world. It inherits from the GameObject class
 * and provides functionality for rendering and updating the wall. Walls are static objects and do not move or interact with other objects.
 */
class Wall : public GameObject
{
public:
  /**
   * @brief Construct a Wall object with the specified position and dimensions.
   *
   * This constructor initializes a Wall object with the given position (x, y) and dimensions (width, height).
   * It also sets the texture of the wall to the default wall texture.
   *
   * @param x The x-coordinate of the wall's position.
   * @param y The y-coordinate of the wall's position.
   * @param width The width of the wall.
   * @param height The height of the wall.
   */
  Wall(int x, int y, int width, int height);

  /**
   * @brief Destructor for the Wall class.
   *
   * This destructor is the default destructor for the Wall class.
   */
  ~Wall();

  /**
   * @brief Update the wall.
   *
   * This method updates the wall. Since walls are static objects,
   * this method does nothing.
   */
  void update() override;

  /**
   * @brief Render the wall.
   *
   * This method renders the wall on the screen using the wall's texture.
   * It copies the texture to the renderer at the position and dimensions of the wall.
   */
  void render() override;
};

#endif
