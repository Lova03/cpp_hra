#ifndef IMAGE_H
#define IMAGE_H

#include <SDL2/SDL.h>
#include "ResourceManager.h"
#include <string>

/**
 * @class Image
 * @brief Represents an Image in the game
 *
 * The Image class is responsible for loading, rendering, and positioning an image on the screen.
 * It utilizes the SDL library for image rendering and provides basic functionality for positioning the image.
 * This class is suitable for displaying static images in the game.
 */
class Image
{
public:
  /**
   * @brief Constructor for the Image class.
   *
   * This constructor initializes the image by loading its texture from a file,
   * and setting its position and dimensions.
   *
   * @param imagePath The path to the image file.
   * @param x The x-coordinate of the image's position.
   * @param y The y-coordinate of the image's position.
   */
  Image(const std::string &imagePath, int x = 0, int y = 0);
  /**
   * @brief Destructor for the Image class.
   *
   * The destructor is default as there are no special cleanup operations needed.
   */
  ~Image();

  /**
   * @brief Renders the image.
   *
   * This function draws the image at its current position on the screen.
   */
  void render();

  /**
   * @brief Sets the position of the image.
   *
   * This function allows you to set the x and y coordinates of the image.
   *
   * @param x The new x-coordinate of the image.
   * @param y The new y-coordinate of the image.
   */
  void setPosition(int x, int y);

private:
  SDL_Texture *texture;
  SDL_Rect position;
};

#endif