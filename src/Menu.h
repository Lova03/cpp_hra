#ifndef MENU_H
#define MENU_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "Button.h"
#include "TextButton.h"
#include "ImageButton.h"
#include "Text.h"
#include "Image.h"
#include <string>
#include <functional>
#include <vector>
#include <memory>

/**
 * @class Menu
 * @brief Represents a menu in the game.
 *
 * The Menu class provides functionality to create and manage menus in the game.
 * It supports various types of buttons, including image buttons and text buttons,
 * as well as text elements and images. The menu can have a background texture or color.
 * It handles user input, rendering, and updating of the menu elements.
 */
class Menu
{
public:
  /**
   * @brief Default constructor for the Menu class.
   *
   * Initializes a Menu object with a null background texture and transparent color.
   */
  Menu();

  /**
   * @brief Destructor for the Menu class.
   *
   * Frees the SDL_Texture resource associated with the Menu's background, if it exists.
   */
  ~Menu();

  /**
   * @brief Handles click events in the Menu.
   *
   * This method is called when a mouse click event is detected. It checks if any of the buttons in the Menu is being hovered,
   * and if so, calls its click handler function.
   *
   * @param mouseX The x-coordinate of the mouse cursor.
   * @param mouseY The y-coordinate of the mouse cursor.
   */
  void handleClick(int mouseX, int mouseY);

  /**
   * @brief Creates a tiled background from a given texture and sets it to the Menu's background.
   *
   * This method generates a tiled background texture for the menu from the given tileset texture.
   * The tileset texture should contain tiles of equal size. The generated background will be made up of randomly chosen tiles from the tileset.
   *
   * @param tilesetTexture The texture from which tiles are taken to generate the background.
   * @param windowWidth The width of the window.
   * @param windowHeight The height of the window.
   */
  void createBackground(SDL_Texture *tilesetTexture, int windowWidth, int windowHeight);

  /**
   * @brief Sets the background image for the Menu from an image file.
   *
   * This method attempts to load an image from the given file path, convert it to an SDL_Texture, and set it as the Menu's background.
   * If there is a problem loading the image file, an error message will be printed to the standard error stream.
   *
   * @param imagePath The file path of the image to set as the Menu's background.
   */
  void setBackground(const std::string &imagePath);

  /**
   * @brief Sets the background color of the Menu.
   *
   * The function creates a texture of the specified color and sets it as the Menu's background.
   *
   * @param color The desired background color.
   */
  void setBackgroundColor(SDL_Color color);

  /**
   * @brief Adds an ImageButton to the Menu.
   *
   * This method creates a new ImageButton object and adds it to the list of buttons in the Menu.
   *
   * @param x The x-coordinate of the button's position.
   * @param y The y-coordinate of the button's position.
   * @param width The width of the button.
   * @param height The height of the button.
   * @param normalImagePath The path to the image to display on the button when it is not being hovered.
   * @param hoveredImagePath The path to the image to display on the button when it is being hovered.
   * @param onClick The function to call when the button is clicked.
   */
  void addImageButton(int x, int y, int width, int height, const std::string &normalImagePath, const std::string &hoveredImagePath, std::function<void()> onClick);

  /**
   * @brief Adds a TextButton to the Menu.
   *
   * This method creates a new TextButton object and adds it to the list of buttons in the Menu.
   *
   * @param x The x-coordinate of the button's position.
   * @param y The y-coordinate of the button's position.
   * @param width The width of the button.
   * @param height The height of the button.
   * @param text The text to display on the button.
   * @param fontPath The path to the font to use for the button's text.
   * @param fontSize The size of the font to use for the button's text.
   * @param textColor The color of the button's text.
   * @param backgroundColor The color of the button's background.
   * @param hoveredBackgroundColor The color of the button's background when it is being hovered.
   * @param borderColor The color of the button's border.
   * @param hoveredBorderColor The color of the button's border when it is being hovered.
   * @param onClick The function to call when the button is clicked.
   */
  void addTextButton(int x, int y, int width, int height, const std::string &text, const std::string &fontPath, int fontSize, SDL_Color textColor, SDL_Color backgroundColor, SDL_Color hoveredBackgroundColor, SDL_Color borderColor, SDL_Color hoveredBorderColor, std::function<void()> onClick);

  /**
   * @brief Adds a Text element to the Menu.
   *
   * This method creates a new Text object and adds it to the list of text elements in the Menu.
   *
   * @param text The text to display.
   * @param fontPath The path to the font to use.
   * @param fontSize The size of the font to use.
   * @param color The color of the text.
   * @param x The x-coordinate of the text's position.
   * @param y The y-coordinate of the text's position.
   */
  void addText(std::string text, std::string fontPath, int fontSize, SDL_Color color, int x, int y);

  /**
   * @brief Adds an Image to the Menu.
   *
   * This method creates a new Image object and adds it to the list of images in the Menu.
   *
   * @param imagePath The path to the image to add.
   * @param x The x-coordinate of the image's position.
   * @param y The y-coordinate of the image's position.
   */
  void addImage(const std::string &imagePath, int x, int y);

  /**
   * @brief Adds a TextButton to the Menu.
   *
   * This method takes ownership of the passed in unique pointer to a TextButton object and adds it to the list of buttons in the Menu.
   *
   * @param button The unique pointer to the TextButton object.
   */
  void addTextButton(std::unique_ptr<TextButton> button);

  /**
   * @brief Adds an ImageButton to the Menu.
   *
   * This method takes ownership of the passed in unique pointer to an ImageButton object and adds it to the list of buttons in the Menu.
   *
   * @param button The unique pointer to the ImageButton object.
   */
  void addImageButton(std::unique_ptr<ImageButton> button);

  /**
   * @brief Adds a Text to the Menu.
   *
   * This method takes ownership of the passed in unique pointer to a Text object and adds it to the list of texts in the Menu.
   *
   * @param text The unique pointer to the Text object.
   */
  void addText(std::unique_ptr<Text> text);

  /**
   * @brief Adds an Image to the Menu.
   *
   * This method takes ownership of the passed in unique pointer to an Image object and adds it to the list of images in the Menu.
   *
   * @param image The unique pointer to the Image object.
   */
  void addImage(std::unique_ptr<Image> image);

  /**
   * @brief Adds a Text to the Menu and returns a pointer to it.
   *
   * This method creates a new Text object, adds it to the list of texts in the Menu, and then returns a pointer to the newly created Text object.
   *
   * @param text The text to display.
   * @param fontPath The path to the font to use.
   * @param fontSize The size of the font to use.
   * @param color The color of the text.
   * @param x The x-coordinate of the text's position.
   * @param y The y-coordinate of the text's position.
   *
   * @return Pointer to the newly created Text object.
   */
  Text *addTextAndGet(std::string text, std::string fontPath, int fontSize, SDL_Color color, int x, int y);

  /**
   * @brief Adds a Text to the Menu and returns a pointer to it.
   *
   * This method takes ownership of the passed in unique pointer to a Text object, adds it to the list of texts in the Menu, and then returns a pointer to the Text object.
   *
   * @param text The unique pointer to the Text object.
   *
   * @return Pointer to the Text object.
   */
  Text *addTextAndGet(std::unique_ptr<Text> text);

  /**
   * @brief Adds an ImageButton to the Menu and returns a pointer to it.
   *
   * This method creates a new ImageButton object, adds it to the list of buttons in the Menu, and then returns a pointer to the newly created ImageButton object.
   *
   * @param x The x-coordinate of the button's position.
   * @param y The y-coordinate of the button's position.
   * @param width The width of the button.
   * @param height The height of the button.
   * @param normalImagePath The path to the image to display when the button is not being hovered.
   * @param hoveredImagePath The path to the image to display when the button is being hovered.
   * @param onClick The function to call when the button is clicked.
   *
   * @return Pointer to the newly created ImageButton object.
   */
  ImageButton *addImageButtonAndGet(int x, int y, int width, int height, const std::string &normalImagePath, const std::string &hoveredImagePath, std::function<void()> onClick);

  /**
   * @brief Renders all elements in the Menu.
   *
   * This includes images, buttons, and text elements.
   */
  void render();

  /**
   * @brief Updates the state of all buttons in the Menu.
   */
  void update();

  /**
   * @brief Renders the Menu's background texture.
   */
  void renderBackground();

private:
  std::vector<std::unique_ptr<Button>> buttons;
  std::vector<ImageButton *> imageButtons;
  std::vector<std::unique_ptr<Text>> texts;
  std::vector<std::unique_ptr<Image>> images;
  SDL_Texture *background;
};

#endif