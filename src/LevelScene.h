#ifndef LEVELSCENE_H
#define LEVELSCENE_H

#include "Player.h"
#include "AI.h"
#include "GameObject.h"
#include "LevelState.h"
#include "Menu.h"
#include "Map.h"
#include "Wall.h"
#include "Text.h"
#include "Castle.h"
#include <string>
#include <memory>
#include <utility>
#include <vector>
#include <string>

/**
 * @class LevelScene
 * @brief Represents a scene for a specific game level.
 *
 * The LevelScene class handles the logic and rendering for a specific game level. It is responsible for
 * loading and validating level data, updating the game state, rendering game objects, handling user input,
 * and managing menus and end-game conditions. It encapsulates the behavior and components of a level,
 * including the map, players, AI, units, castles, resources, menus, and end-game messages.
 */
class LevelScene
{
public:
  /**
   * @brief Constructs a new LevelScene object.
   *
   * Initializes a new level scene with the given level data (name and file path),
   * creating menus, loading textures and settings up the game state.
   *
   * @param levelData A pair containing the name and file path of the level to load.
   */
  LevelScene(const std::pair<std::string, std::string> &levelData);

  /**
   * @brief Destroys the Level Scene object
   *
   */
  ~LevelScene() = default;

  /**
   * @brief Loads a level from a text file and initializes the map.
   *
   * The method reads a level file specified by the path, validates its content and,
   * if the content is valid, initializes the map and game objects accordingly.
   *
   * @param levelFilePath Path to the level file.
   * @return Returns true if the level was successfully loaded; otherwise, false.
   */
  bool loadLevel(const std::string &levelFilePath);

  /**
   * @brief Validates the area around a specific map character.
   *
   * This method checks if the area around a character at position (topLeftX, topLeftY) in the map
   * has the expected size and character. It is used to verify the validity of castles and resources.
   *
   * @param topLeftX The x-coordinate of the top-left corner of the area to validate.
   * @param topLeftY The y-coordinate of the top-left corner of the area to validate.
   * @param size The size of the area to validate.
   * @param expectedCharacter The expected character that should occupy the area.
   * @return Returns true if the area is valid; otherwise, false.
   */
  bool isAreaValid(int topLeftX, int topLeftY, int size, char expectedCharacter);

  /**
   * @brief Updates the state of the level based on loaded game state.
   *
   * This method uses the saved game state to enable talents, set units, set castle health,
   * and set resources for both the human player and AI players.
   */
  void updateState();

  /**
   * @brief Saves the current state of the level.
   *
   * This method saves the current state of the level including units, castles, players
   * and their resources to a file for future continuation of the game.
   */
  void saveCurrentLevel();

  /**
   * @brief Updates the level scene in each frame of the game.
   *
   * This function controls the game's state, updates the states of all objects in the level, and determines whether the player has won or lost.
   * If the game is over, it prepares and shows the appropriate end game message.
   */
  void update();

  /**
   * @brief Renders the level scene.
   *
   * This function controls the drawing of all objects in the level scene including walls, resources, AI, units, and player.
   * It also renders the menus according to the game's state.
   */
  void render();

  /**
   * @brief Handles the input for the level scene.
   *
   * This function processes user inputs that are relevant for the level scene.
   * It handles mouse and keyboard inputs, passing these inputs to the player and menus as needed.
   * It also checks for a quit event or a press on the escape key to end the game.
   *
   * @param event The SDL_Event that holds the event data that needs to be handled.
   */
  void handleInput(SDL_Event &event);

  /**
   * @brief Gets the map of the level scene.
   *
   * This method returns a reference to the map object of the level scene.
   *
   * @return A reference to the map object of the level scene.
   */
  static Map &getMap() { return *map; };

private:
  std::string name;
  static std::unique_ptr<Map> map;
  std::vector<std::string> mapData;
  bool success;
  std::unique_ptr<Menu> levelMenu;
  std::unique_ptr<Menu> endMenu;
  bool talentsVisible;
  bool gameOver;
  bool playerWon;
  Text *endMessage;

  std::unique_ptr<Player> player;
  std::vector<std::unique_ptr<AI>> ais;

  std::vector<std::unique_ptr<Unit>> allUnits;
  std::vector<Unit *> unitsToRemove;
  std::vector<std::unique_ptr<Wall>> allWalls;
  std::vector<std::unique_ptr<Resource>> allResources;
  std::vector<Castle *> allCastles;

  LevelState *state;
};

#endif