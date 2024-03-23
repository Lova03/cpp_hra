#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <SDL2/SDL.h>
#include <utility>
#include <string>

/**
 * @class GameObject
 * @brief Game Object Base Class
 *
 * The GameObject class represents a generic object in the game.
 * It provides basic functionality for position, size, rendering, and collision detection.
 * This class is intended to be inherited by specific game objects.
 */
class GameObject
{
public:
  /**
   * @brief Constructor for the GameObject class.
   *
   * This constructor initializes the dimensions of the object and sets its texture to null.
   *
   * @param x The x-coordinate of the object.
   * @param y The y-coordinate of the object.
   * @param width The width of the object.
   * @param height The height of the object.
   */
  GameObject(int x, int y, int width, int height);

  /**
   * @brief Updates the GameObject.
   *
   * This function is intended to be overridden by derived classes to provide
   * specific behavior during the game's update loop.
   * The base implementation is empty because a general GameObject does not have default behavior.
   */
  virtual void update();

  /**
   * @brief Renders the GameObject.
   *
   * If the texture of the GameObject is not null, it copies the texture to the renderer,
   * at the position specified by the object's rectangle (objectRect).
   */
  virtual void render();

  /**
   * @brief Sets the position of the GameObject.
   *
   * This function allows you to set the x and y coordinates of the object.
   *
   * @param x The new x-coordinate of the object.
   * @param y The new y-coordinate of the object.
   */
  void setPosition(int x, int y);

  /**
   * @brief Retrieves the position of the GameObject.
   *
   * This function returns a pair representing the x and y coordinates of the GameObject.
   *
   * @return A std::pair where first is the x-coordinate and second is the y-coordinate.
   */
  std::pair<int, int> getPosition() const;

  /**
   * @brief Sets the size of the GameObject.
   *
   * This function allows you to set the width and height of the object.
   *
   * @param width The new width of the object.
   * @param height The new height of the object.
   */
  void setSize(int width, int height);

  /**
   * @brief Retrieves the size of the GameObject.
   *
   * This function returns a pair representing the width and height of the GameObject.
   *
   * @return A std::pair where first is the width and second is the height.
   */
  std::pair<int, int> getSize() const;

  /**
   * @brief Checks if the GameObject collides with another GameObject.
   *
   * This function checks if the bounding boxes of two GameObjects intersect,
   * indicating a collision between the two objects.
   *
   * @param other The other GameObject to check collision with.
   * @return true if the GameObjects collide, false otherwise.
   */
  bool checkCollision(const GameObject &other) const;

  /**
   * @brief Sets the texture of the GameObject.
   *
   * This function allows you to set the texture of the GameObject using a file path.
   *
   * @param filePath The file path to the texture.
   */
  void setTexture(const std::string &filePath);

  SDL_Rect objectRect;

protected:
  SDL_Texture *texture;
};

#endif