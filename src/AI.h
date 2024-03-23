#ifndef AI_H
#define AI_H

#include "Unit.h"
#include "Resource.h"
#include "Castle.h"
#include "TalentManager.h"
#include "LevelState.h"
#include <vector>

/**
 * @class AI
 * @brief Represents the AI player in the game.
 *
 * The AI class is responsible for controlling the computer player in the game. It manages the AI's castle,
 * units, resources, talents, and decision-making process. The AI makes decisions on unit movements, responses
 * to threats, and unlocking talents based on game conditions and strategies.
 */
class AI
{
public:
  /**
   * @brief AI constructor.
   * This is used to initialize an AI object with position (x,y), ID and game references.
   * The AI object represents the computer player in the game.
   *
   * @param x X position of the AI's castle.
   * @param y Y position of the AI's castle.
   * @param id Unique identifier for the AI.
   * @param allUnits Reference to all units in the game.
   * @param unitsToRemove Reference to units that need to be removed from the game.
   * @param allWalls Reference to all walls in the game.
   * @param allResources Reference to all resources in the game.
   * @param allCastles Reference to all castles in the game.
   */
  AI(int x, int y, int id, std::vector<std::unique_ptr<Unit>> &allUnits, std::vector<Unit *> &unitsToRemove, std::vector<std::unique_ptr<Wall>> &allWalls, std::vector<std::unique_ptr<Resource>> &allResources, std::vector<Castle *> &allCastles);

  /**
   * @brief Default destructor.
   */
  ~AI();

  /**
   * @brief Updates the state of the AI.
   *
   * Calls the update function of the castle and decides which action the AI should take next.
   */
  void update();

  /**
   * @brief Renders the AI's castle.
   */
  void render();

  /**
   * @brief Return AI's castle
   *
   * @return Castle& The castle of AI
   */
  Castle &getCastle();

  /**
   * @brief Adds unit with specified parameters to AI's castle
   *
   * @param x The x coordinate of the new unit
   * @param y The y coordinate of the new unit
   * @param type Type of the unit to add
   * @param health Health of the unit to add
   */
  void addUnit(int x, int y, const std::string &type, int health);

  /**
   * @brief Adds specified amount of crystals
   *
   * @param amount The amount of crystals to add
   */
  void addCrystals(int amount);

  /**
   * @brief Adds specified amount of wood
   *
   * @param amount The amount of wood to add
   */
  void addWood(int amount);

  /**
   * @brief Returns the amount of crystals this AI has
   *
   * @return int Amount of crystals
   */
  int getCrystals() const;

  /**
   * @brief Returns the amount of wood this AI has
   *
   * @return int Amount of wood
   */
  int getWood() const;

  /**
   * @brief Returns pointer to AI's TalentManager
   *
   * @return TalentManager* Pointer to TalentManager of this AI
   */
  TalentManager *getTalentManager();

  /**
   * @brief Returns the ID of this AI player
   *
   * @return int The ID of this AI
   */
  int getId() const;

private:
  std::vector<Unit *> selectedUnits;
  std::vector<std::unique_ptr<Unit>> &allUnits;
  std::vector<Castle *> &allCastles;
  std::vector<std::unique_ptr<Resource>> &allResources;

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

  uint32_t spawnInterval;

  int id;
  Castle castle;
  int lastCastleHP;

  /**
   * @brief Determines the action that the AI should take next.
   *
   * This includes moving units, responding to threats, and unlocking talents.
   */
  void decideAction();

  /**
   * @brief Checks area around a object in specified radius for units
   *
   * @param target The object to check tiles around
   * @param tiles The amount of tiles around the object to check (radius)
   * @param friendly If true, looks for friendly units. If false looks for enemy units
   * @param type The type of unit to look for
   * @return true If it found any unit in the radius
   * @return false If none units were found in the radius
   */
  bool checkAround(const GameObject &target, int tiles, bool friendly, std::string type = "any");

  /**
   * @brief Gets the coordinates of unit around a object
   *
   * @param target The object to check tiles around
   * @param tiles The amount of tiles around the object to check (radius)
   * @param friendly If true, looks for friendly units. If false looks for enemy units
   * @param type The type of unit to look for
   * @return std::pair<int, int> The coordinates of found unit.
   */
  std::pair<int, int> getCoordOfUnitAround(const GameObject &target, int tiles, bool friendly, std::string type);

  /**
   * @brief Picks random tile around an object
   *
   * @param target The object that we want to check for random tile
   * @return std::pair<int, int> Coordinates of the random tile
   */
  std::pair<int, int> pickRandomTileAround(const GameObject &target);
};

#endif
