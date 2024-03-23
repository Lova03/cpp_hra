#ifndef LEVELSTATE_H
#define LEVELSTATE_H

#include <string>
#include <utility>
#include <vector>

/**
 * @brief This class represents the state of a game level.
 *
 * LevelState is used to store all the important information about the state of a game level. This includes
 * information about all units, castles, talents, and players in the level.
 *
 * It is particularly useful for saving and loading game states.
 */
class LevelState
{
public:
  std::string levelName;

  /**
   * @brief Represents information about a game unit.
   *
   * This structure holds the ownerId, health, type, and coordinates of a game unit.
   */
  struct UnitInfo
  {
    int ownerId;
    int health;
    std::string type;
    std::pair<int, int> coords;
  };

  std::vector<UnitInfo> units;

  /**
   * @brief Represents information about a castle in the game.
   *
   * This structure holds the ownerId and health of a castle.
   */
  struct CastleInfo
  {
    int ownerId;
    int health;
  };

  std::vector<CastleInfo> castles;

  /**
   * @brief Represents information about a talent in the game.
   *
   * This structure holds the ownerId, talentName, and unlocked status of a talent.
   */
  struct TalentInfo
  {
    int ownerId;
    std::string talentName;
    bool unlocked;
  };

  std::vector<TalentInfo> talents;

  /**
   * @brief Represents information about a player in the game.
   *
   * This structure holds the id, crystals, and wood of a player.
   */
  struct PlayerInfo
  {
    int id;
    int crystals;
    int wood;
  };

  std::vector<PlayerInfo> players;
};

#endif
