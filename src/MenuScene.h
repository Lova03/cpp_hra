#ifndef MENUSCENE_H
#define MENUSCENE_H

#include "Menu.h"
#include <memory>

/**
 * @class MenuScene
 * @brief Represents a scene in the game that displays the main menu.
 *
 * The MenuScene class is responsible for managing and displaying the main menu in the game.
 * It handles user input, updates the menu, and renders it on the screen.
 */
class MenuScene
{
public:
  /**
   * @brief Default constructor for the MenuScene class
   *
   * Initialises the MenuScene, creates the main menu and adds various images and buttons to it.
   */
  MenuScene();

  /**
   * @brief Default destructor for the MenuScene class.
   */
  ~MenuScene();

  /**
   * @brief Handles the user input for the menu scene.
   *
   * It handles various events such as quitting the game, mouse clicks and keyboard events.
   *
   * @param event A SDL_Event object containing the details of the event.
   */
  void handleInput(SDL_Event &event);

  /**
   * @brief Update function for the menu scene.
   *
   * It calls the update function of the main menu object.
   */
  void update();

  /**
   * @brief Render function for the menu scene.
   *
   * It renders the main menu and its background.
   */
  void render();

private:
  std::unique_ptr<Menu> mainMenu;
};

#endif
