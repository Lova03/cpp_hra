#ifndef SAVE_H
#define SAVE_H

#include <string>
#include <map>
#include "LevelState.h"

/**
 * @class Save
 * @brief Manages saving and loading game progress.
 *
 * The Save class handles the saving and loading of game progress. It provides methods to save and retrieve the state of individual levels.
 * The saved states are stored in a file specified by the file path provided during construction.
 */
class Save
{
public:
  /**
   * @brief Constructs a new Save object.
   *
   * @param filePath Path of the save file.
   */
  Save(const std::string &filePath);

  /**
   * @brief Default destructor for the Save class.
   */
  ~Save();

  /**
   * @brief Get the saved state of a specific level.
   *
   * @param levelName Name of the level.
   * @return Pointer to the LevelState of the level. nullptr if the level is not found.
   */
  LevelState *getLevelState(const std::string &levelName);

  /**
   * @brief Save the current state of a level.
   *
   * This function will overwrite the current saved state of the level.
   * After updating the level state, it also saves it to the file.
   *
   * @param levelState Current state of the level.
   */
  void saveLevelState(const LevelState &levelState);

  /**
   * @brief Initialize the save system with default (empty) level states.
   *
   * Default level states are defined by the levels in the game config.
   */
  void init();

  /**
   * @brief Load saved states from the save file.
   *
   * This function reads the save file and updates the current level states accordingly.
   * If the file cannot be opened, it will output an error message.
   */
  void load();

private:
  std::string filePath;
  std::map<std::string, LevelState> levelStates;

  /**
   * @brief Write current saved states to the save file.
   *
   * This function writes the current level states to the save file.
   * If the file cannot be opened, it will output an error message.
   */
  void save();
};

#endif // SAVE_H
