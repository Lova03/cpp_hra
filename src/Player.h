#ifndef PLAYER_H
#define PLAYER_H

#include "Unit.h"
#include "Castle.h"
#include "Resource.h"
#include "Menu.h"
#include "Text.h"
#include "TalentManager.h"
#include <vector>
#include <memory>

/**
 * @class Player
 * @brief Represents a player in the game.
 *
 * The Player class manages the state and actions of a player in the game. It controls units, resources, menus, and talents.
 * It handles user input, updates the player's state, and renders the player's menus and objects.
 */
class Player
{
public:
  /**
   * @brief Constructs a new Player instance.
   *
   * Sets up the player with the given properties and initializes their resources and menus.
   *
   * @param x The x-coordinate of the player's castle.
   * @param y The y-coordinate of the player's castle.
   * @param id The ID of the player.
   * @param talentsVisible A reference to a bool indicating whether the talent menu is visible.
   * @param allUnits A reference to the vector containing all units.
   * @param unitsToRemove A reference to the vector containing units to be removed.
   * @param allWalls A reference to the vector containing all walls.
   * @param allResources A reference to the vector containing all resources.
   * @param allCastles A reference to the vector containing all castles.
   */
  Player(int x, int y, int id, bool &talentsVisible, std::vector<std::unique_ptr<Unit>> &allUnits, std::vector<Unit *> &unitsToRemove, std::vector<std::unique_ptr<Wall>> &allWalls, std::vector<std::unique_ptr<Resource>> &allResources, std::vector<Castle *> &allCastles);

  /**
   * @brief Default destructor for the Player class.
   */
  ~Player();

  /**
   * @brief Update the state of the player.
   *
   * This includes updating the select and talent menus, as well as updating the wood and crystal texts in the menu.
   */
  void update();

  /**
   * @brief Renders the player's menus.
   */
  void render();

  /**
   * @brief Handles the user input for the player.
   *
   * This includes clicking on menus and selecting units.
   *
   * @param event A SDL_Event object containing the details of the event.
   */
  void handleInput(const SDL_Event &event);

  /**
   * @brief Selects units within a given area.
   */
  void selectUnits();

  /**
   * @brief Sets the target coords for the selected units.
   *
   * @param targetX The x-coordinate of the target ( end of path ).
   * @param targetY The y-coordinate of the target ( end of path ).
   */
  void setUnitsTarget(int targetX, int targetY);

  /**
   * @brief Spawns a new unit at the given position with the given type and health.
   *
   * @param x The x-coordinate of the unit.
   * @param y The y-coordinate of the unit.
   * @param type The type of the unit.
   * @param health The health of the unit.
   */
  void addUnit(int x, int y, const std::string &type, int health);

  /**
   * @brief Adds crystals to the player's resources.
   *
   * @param amount The amount of crystals to add.
   */
  void addCrystals(int amount);

  /**
   * @brief Adds wood to the player's resources.
   *
   * @param amount The amount of wood to add.
   */
  void addWood(int amount);

  /**
   * @brief Returns the player's crystal count.
   *
   * @return The number of crystals the player has.
   */
  int getCrystals() const;

  /**
   * @brief Returns the player's wood count.
   *
   * @return The number of wood the player has.
   */
  int getWood() const;

  /**
   * @brief Returns a pointer to the player's talent manager.
   *
   * @return A pointer to the player's talent manager.
   */
  TalentManager *getTalentManager();

  /**
   * @brief Returns a vector containing the currently selected units.
   *
   * @return A vector of pointers to the selected units.
   */
  std::vector<Unit *> &getSelectedUnits();

  /**
   * @brief Returns a reference to the player's castle.
   *
   * @return A reference to the player's castle.
   */
  Castle &getCastle();

  /**
   * @brief Sets the ID of the player.
   *
   * @param id The new ID of the player.
   */
  void setId(int id);

  /**
   * @brief Returns the player's ID.
   *
   * @return The ID of the player.
   */
  int getId() const;

private:
  std::vector<Unit *> selectedUnits;
  std::vector<std::unique_ptr<Unit>> &allUnits;
  std::unique_ptr<Menu> talentsMenu;
  std::unique_ptr<Menu> selectMenu;

  std::unique_ptr<TalentManager> talentManager;

  float speedMultiplier;
  float healthMultiplier;
  float spawnRateMultiplier;
  float hasteMultiplier;
  int baseAttackDamage;
  uint32_t baseAttackSpeed;
  uint32_t gatherRate;
  int wood;
  int crystals;
  int lastWood;
  int lastCrystals;

  Text *woodText;
  Text *crystalsText;

  uint32_t spawnInterval;

  int id;
  Castle castle;

  int startX;
  int startY;

  int endX;
  int endY;

  bool isControlling;
  bool &talentsVisible;
};

#endif
