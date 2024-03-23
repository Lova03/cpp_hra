#ifndef LEVELSELECTSCENE_H
#define LEVELSELECTSCENE_H

#include "Menu.h"
#include <memory>

/**
 * @class LevelSelectScene
 * @brief Represents the scene for selecting a game level.
 *
 * The LevelSelectScene class is responsible for managing the level select menu and handling user interactions
 * related to level selection. It initializes the level select menu, positions its buttons based on the window size,
 * and handles user input events. It provides methods to update and render the level select scene.
 */
class LevelSelectScene
{
public:
  /**
   * @brief Constructs a new Level Select Scene.
   *
   * The constructor is responsible for initializing the level select menu and setting up its buttons and actions.
   * It also positions the level select menu buttons based on the window size and the level data available in the game.
   */
  LevelSelectScene();

  /**
   * @brief Destroys the Level Select Scene.
   *
   * This is the default destructor for the LevelSelectScene class.
   */
  ~LevelSelectScene();

  /**
   * @brief Handles the user input for the level select scene.
   *
   * This method handles all user inputs including mouse and keyboard events. It delegates
   * the input handling to the level select menu. It also handles quit events and escape key events to stop the game.
   *
   * @param event The SDL event to handle.
   */
  void handleInput(SDL_Event &event);

  /**
   * @brief Updates the level select scene.
   *
   * This method is responsible for updating the level select menu.
   */
  void update();

  /**
   * @brief Renders the level select scene.
   *
   * This method is responsible for rendering the level select menu and its background.
   */
  void render();

private:
  std::unique_ptr<Menu> levelSelectMenu;
};

#endif
