#ifndef IMAGEBUTTON_H
#define IMAGEBUTTON_H

#include "Button.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <memory>
#include <functional>
#include <string>

/**
 * @class ImageButton
 * @brief Represents an Image Button in the game
 *
 * The ImageButton class extends the functionality of the Button class by adding images
 * for different button states (normal and hovered). It provides the ability to display
 * different textures based on the button's state and handles the behavior of the button
 * when clicked or hovered over.
 */
class ImageButton : public Button
{
public:
  /**
   * @brief Constructor for the ImageButton class.
   *
   * The constructor initializes the ImageButton by loading its textures, setting its position, dimensions,
   * and specifying the behavior when clicked.
   *
   * @param x The x-coordinate of the ImageButton's position.
   * @param y The y-coordinate of the ImageButton's position.
   * @param width The width of the ImageButton.
   * @param height The height of the ImageButton.
   * @param normalImagePath The path to the image file used when the button is in the normal state.
   * @param hoverImagePath The path to the image file used when the button is in the hovered state.
   * @param onClick The function to be executed when the ImageButton is clicked.
   */
  ImageButton(int x, int y, int width, int height, const std::string &normalImagePath, const std::string &hoverImagePath, std::function<void()> onClick);

  /**
   * @brief Destructor for the ImageButton class.
   *
   * The destructor frees the SDL cursor if it was previously created.
   */
  ~ImageButton();

  /**
   * @brief Renders the ImageButton.
   *
   * This function draws the ImageButton at its current position on the screen.
   */
  void render() override;

  /**
   * @brief Updates the ImageButton's state.
   *
   * The function checks if the mouse is hovering over the button and changes its state and cursor accordingly.
   */
  void update() override;

  /**
   * @brief Sets the hover texture of the ImageButton.
   *
   * @param path The path to the image file used when the button is in the hovered state.
   */
  void setHoverTexture(const std::string &path);

  /**
   * @brief Sets the current texture of the ImageButton.
   *
   * @param path The path to the image file used as the current texture of the button.
   */
  void setCurrentTexture(const std::string &path);

  /**
   * @brief Sets the normal texture of the ImageButton.
   *
   * @param path The path to the image file used when the button is in the normal state.
   */
  void setNormalTexture(const std::string &path);

private:
  SDL_Texture *currentTexture;
  SDL_Texture *normalTexture;
  SDL_Texture *hoverTexture;
};

#endif