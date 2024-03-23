#ifndef TEXT_H
#define TEXT_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>

/**
 * @class Text
 * @brief Represents a rendered text on the screen.
 *
 * The Text class represents a rendered text on the screen. It provides functionality to create, update,
 * and render text using a specified font, font size, color, and position.
 */
class Text
{
public:
  /**
   * @brief Constructor for the Text class.
   *
   * This constructor creates a text object that can be rendered on the screen.
   * It takes the text string, font path, font size, color, and initial position as input,
   * creates a texture from the text string, and sets the width and height of the rectangle
   * that represents the text's position and dimensions on the screen.
   *
   * @param text The string that will be rendered.
   * @param fontPath The path to the font file.
   * @param fontSize The size of the font.
   * @param color The color of the text.
   * @param x The x-coordinate of the top-left corner of the text.
   * @param y The y-coordinate of the top-left corner of the text.
   */
  Text(std::string text, std::string fontPath, int fontSize, SDL_Color color, int x, int y);

  /**
   * @brief Destructor for the Text class.
   *
   * This destructor destroys the texture that was created for the text.
   */
  ~Text();

  /**
   * @brief Get the dimensions of the text.
   *
   * This function returns a rectangle that represents the text's position
   * and dimensions on the screen.
   *
   * @return SDL_Rect The rectangle that represents the text's position and dimensions.
   */
  SDL_Rect getDimensions() const;

  /**
   * @brief Set the position of the text.
   *
   * This function sets the position of the top-left corner of the text on the screen.
   *
   * @param x The new x-coordinate of the top-left corner of the text.
   * @param y The new y-coordinate of the top-left corner of the text.
   */
  void setPosition(int x, int y);

  /**
   * @brief Update the text string.
   *
   * This function updates the text string that will be rendered on the screen,
   * and re-creates the texture that represents the text.
   *
   * @param newText The new text string.
   */
  void setText(std::string newText);

  /**
   * @brief Render the text.
   *
   * This function renders the text on the screen at its current position,
   * with its current dimensions, color, and string.
   */
  void render();

private:
  std::string text;
  SDL_Color color;
  std::string fontPath;
  int fontSize;
  SDL_Texture *texture;
  SDL_Rect rect;
};

#endif