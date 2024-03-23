#ifndef TEXTBUTTON_H
#define TEXTBUTTON_H

#include "Button.h"
#include "Text.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <memory>
#include <functional>
#include <string>

/**
 * @class TextButton
 * @brief Represents a clickable text button.
 *
 * The TextButton class represents a clickable text button that can be rendered on the screen.
 * It inherits from the Button class and adds functionality to display text on the button.
 * The button can change its appearance when hovered over and perform a specified action when clicked.
 */
class TextButton : public Button
{
public:
  /**
   * @brief Constructor for the TextButton class.
   *
   * This constructor creates a text button with defined dimensions and position, color configurations,
   * and action to be performed on clicking the button. It also initializes the text element that will be displayed on the button.
   *
   * @param x The x-coordinate of the top-left corner of the button.
   * @param y The y-coordinate of the top-left corner of the button.
   * @param width The width of the button.
   * @param height The height of the button.
   * @param text The string to be displayed on the button.
   * @param fontPath The path to the font file for the text on the button.
   * @param fontSize The size of the font for the text on the button.
   * @param textColor The color of the text.
   * @param backgroundColor The color of the button when it's not hovered.
   * @param hoveredBackgroundColor The color of the button when it's hovered.
   * @param borderColor The color of the button's border when it's not hovered.
   * @param hoveredBorderColor The color of the button's border when it's hovered.
   * @param onClick The function to be called when the button is clicked.
   */
  TextButton(int x, int y, int width, int height, const std::string &text, const std::string &fontPath, int fontSize, SDL_Color textColor, SDL_Color backgroundColor, SDL_Color hoveredBackgroundColor, SDL_Color borderColor, SDL_Color hoveredBorderColor, std::function<void()> onClick);

  /**
   * @brief Destructor for the TextButton class.
   *
   * This destructor frees the cursor if it is not null.
   */
  ~TextButton();

  /**
   * @brief Render the button.
   *
   * This function sets the rendering color to the button's current border color and renders the border,
   * then sets the rendering color to the button's current background color and renders the background,
   * and finally, renders the text on the button.
   */
  void render() override;

  /**
   * @brief Update the button's state.
   *
   * This function checks if the button is being hovered over. If the button is being hovered over,
   * it changes the button's background and border color to the hovered colors, and sets the cursor to a hand cursor.
   * If the button was being hovered over in the previous frame but isn't being hovered over now,
   * it changes the button's background and border color back to the normal colors, and resets the cursor.
   */
  void update() override;

private:
  std::unique_ptr<Text> textElement;
  SDL_Color currentBackground;
  SDL_Color currentBorder;
  SDL_Color backgroundColor;
  SDL_Color hoveredBackgroundColor;
  SDL_Color borderColor;
  SDL_Color hoveredBorderColor;
};

#endif