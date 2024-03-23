#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <vector>
#include <memory>
#include <utility>
#include <string>
#include "ResourceManager.h"
#include "Save.h"
#include "MenuScene.h"
#include "LevelSelectScene.h"
#include "LevelScene.h"
#include "LevelState.h"

/**
 * @enum GameState
 * @brief Defines the different states the game can be in at any given time.
 *
 * The game state determines what scene should be active and how the game should respond to events.
 */
enum GameState
{
  MENU,         /**< Represents the game state where the main menu is active. */
  LEVEL_SELECT, /**< Represents the game state where the level selection screen is active. */
  LEVEL         /**< Represents the game state where an actual level is being played. */
};

/**
 * @class Game
 * @brief Game Management Class
 *
 * The Game class is responsible for managing the game state, handling events, updating game objects, and rendering the game.
 * It initializes the game, runs the game loop, and cleans up resources when the game ends.
 * Only one class instance should be created.
 */
class Game
{
public:
  /**
   * @brief Constructs a new empty Game object
   *
   */
  Game(){};

  /**
   * @brief Destroys the Game object
   *
   */
  ~Game(){};

  /**
   * @brief Initializes the game by setting up SDL and creating a window and renderer.
   * Also initializes SDL_image and SDL_ttf, loads game configuration, and main scene and level select scene.
   * @param title The title of the game window.
   * @param xpos The x position of the game window.
   * @param ypos The y position of the game window.
   * @param width The width of the game window.
   * @param height The height of the game window.
   * @param fullscreen Whether or not to run the game in fullscreen.
   * @return True if initialization was successful, false otherwise.
   */
  bool init(const char *title, int xpos, int ypos, int width, int height, bool fullscreen);

  /**
   * @brief Runs the game loop. Handles events, updates the game state, and renders the game until the game stops running.
   */
  void run();

  /**
   * @brief Handles SDL events by polling them and delegating handling to the active scene.
   */
  void handleEvents();

  /**
   * @brief Updates the game state by delegating to the update function of the active scene.
   */
  void update();

  /**
   * @brief Renders the game by delegating to the render function of the active scene.
   */
  void render();

  /**
   * @brief Frees all resources and destroys the window and renderer when the game ends.
   */
  void cleanup();

  /**
   * @brief Changes the game state to the new specified state, potentially initiating a new level.
   * @param newState The new game state to switch to.
   * @param levelData A pair containing data for the new level (if the new state is LEVEL).
   */
  static void changeState(GameState newState, const std::pair<std::string, std::string> &levelData = {"", ""});

  /**
   * @brief Sets the SDL Cursor.
   * @param cursor Pointer to the SDL Cursor.
   */
  static void setCursor(SDL_Cursor *cursor);

  /**
   * @brief Resets the SDL Cursor to the default one.
   */
  static void resetCursor();

  static GameState currentState;
  static Save save;

  static std::vector<std::pair<std::string, std::string>> levels;
  static std::vector<LevelState> levelStates;

  static std::vector<std::pair<std::string, std::pair<int, int>>> talents;

  static std::unique_ptr<MenuScene> mainMenuScene;
  static std::unique_ptr<LevelSelectScene> levelSelectScene;
  static std::unique_ptr<LevelScene> currentLevelScene;

  static SDL_Renderer *renderer;
  static SDL_Window *window;
  static ResourceManager resourceManager;
  static bool isRunning;
};

#endif // GAME_H