#ifndef BUTTON_H
#define BUTTON_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <memory>
#include <functional>
#include <string>

/**
 * @class Button
 * @brief User Interface Button
 *
 * The Button class represents a graphical user interface button that can be interacted with by the user.
 * It provides functionality for rendering, handling mouse events, and invoking a callback function when clicked.
 * This is an abstract base class and must be subclassed to provide concrete rendering and updating implementations.
 */
class Button
{
public:
  /**
   * @brief Constructor for the Button class.
   * Initializes the Button object with the position, size and click handler.
   * @param x The x coordinate of the Button's position.
   * @param y The y coordinate of the Button's position.
   * @param width The width of the Button.
   * @param height The height of the Button.
   * @param onClick A function to be called when the Button is clicked.
   */
  Button(int x, int y, int width, int height, std::function<void()> onClick);

  /**
   * @brief Virtual destructor for the Button class.
   */
  virtual ~Button() = default;

  /**
   * @brief Checks if the Button is currently being hovered over by the mouse.
   * This is determined by whether the mouse's current position is within the Button's bounds.
   * @param mouseX The current x coordinate of the mouse cursor.
   * @param mouseY The current y coordinate of the mouse cursor.
   * @return True if the Button is being hovered over, false otherwise.
   */
  bool isHovered(int mouseX, int mouseY);

  /**
   * @brief Sets whether the Button can be hovered over.
   * If set to false, the Button will not respond to mouse hover events.
   * @param val The new hoverable state of the Button.
   */
  void setHoverable(bool val);

  /**
   * @brief Sets a new function to be called when the Button is clicked.
   * This replaces the previous onClick function.
   * @param newOnClick The new function to be called when the Button is clicked.
   */
  void setOnClick(std::function<void()> newOnClick);

  /**
   * @brief Calls the Button's onClick function.
   * This function should be called when a mouse click event is detected on the Button.
   */
  void handleMouseClick();

  /**
   * @brief Pure virtual render method.
   * Derived classes must provide an implementation to handle how the button is drawn.
   */
  virtual void render() = 0;

  /**
   * @brief Pure virtual update method.
   * Derived classes must provide an implementation to handle how the button is updated, e.g., in response to user actions.
   */
  virtual void update() = 0;

protected:
  SDL_Rect rect;
  std::function<void()> onClick;
  SDL_Cursor *handCursor;
  bool wasHovered;
  bool hoverable;
};

#endif