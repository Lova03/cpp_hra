#ifndef TALENTMANAGER_H
#define TALENTMANAGER_H

#include <unordered_map>
#include <unordered_set>
#include <string>
#include <vector>
#include <utility>
#include "Menu.h"
#include "ImageButton.h"

/**
 * @struct Talent
 *
 * @brief Represents an unlockable talent in the game.
 *
 * This struct encapsulates the properties of a talent that can be unlocked in the game. A talent has a name, a price (represented as a pair of crystals and wood), dependencies (other talents that need to be unlocked first), a new value that gets applied when the talent is unlocked, and an associated ImageButton in the UI.
 */
struct Talent
{
  std::string name;                      ///< The name of the talent.
  std::pair<int, int> price;             ///< The price to unlock the talent. Represented as a pair: <crystals, wood>.
  std::vector<std::string> dependencies; ///< The names of other talents that need to be unlocked before this one can be.
  float newValue;                        ///< The value that gets applied when the talent is unlocked.
  ImageButton *button;                   ///< Pointer to the ImageButton in the UI associated with this talent.
};

/**
 * @class TalentManager
 * @brief Manages talents in the game.
 *
 * The TalentManager class is responsible for managing talents in the game.
 * It provides functionality to register talents, unlock and enable talents, and apply their effects.
 * The class also handles the graphical representation of talents in the user interface.
 */
class TalentManager
{
public:
  /**
   * @brief Get the default talent value.
   *
   * This function takes a string (the name of a talent) as input and returns
   * the default value for that talent.
   *
   * @param talentName A string that specifies the talent.
   * @return float The default value of the talent.
   */
  static float getDefaultTalentValue(const std::string &talentName);

  /**
   * @brief Constructor for the TalentManager class.
   *
   * This constructor initializes the TalentManager object, associating it with resources, multipliers,
   * and a renderMenu object. It also initiates the process of registering talents.
   *
   * @param crystals Reference to the crystal count.
   * @param wood Reference to the wood count.
   * @param speedMultiplier Reference to the speed multiplier.
   * @param healthMultiplier Reference to the health multiplier.
   * @param spawnRateMultiplier Reference to the spawn rate multiplier.
   * @param hasteMultiplier Reference to the haste multiplier.
   * @param baseAttackDamage Reference to the base attack damage.
   * @param renderMenu Pointer to the Menu object.
   */
  TalentManager(int &crystals, int &wood, float &speedMultiplier, float &healthMultiplier, float &spawnRateMultiplier, float &hasteMultiplier, int &baseAttackDamage, Menu *renderMenu = nullptr);

  /**
   * @brief Destroy the Talent Manager object
   *
   */
  ~TalentManager();

  /**
   * @brief Registers a talent to the TalentManager.
   *
   * This function attempts to register a talent, defined by the 'talent' parameter, into the manager. It also handles the talent dependencies and the creation of interactive elements for a graphical interface.
   *
   * @param talent The talent to be registered.
   */
  void registerTalent(Talent &talent);

  /**
   * @brief Unlocks a talent.
   *
   * This function tries to unlock a talent specified by its name. If the talent is not found or cannot be unlocked, it provides error handling.
   *
   * @param name The name of the talent to be unlocked.
   * @return bool Returns true if the talent was successfully unlocked, false otherwise.
   */
  bool unlockTalent(const std::string &name);

  /**
   * @brief Enable a talent.
   *
   * This function enables a talent specified by its name. The talent's name is case-insensitive.
   *
   * @param name The name of the talent to be enabled.
   * @return bool Returns true if the talent was successfully enabled, false otherwise.
   */
  bool enableTalent(const std::string &name);

  /**
   * @brief Get all affordable talents.
   *
   * This function iterates over all locked talents and returns a list of talents that can be unlocked.
   *
   * @return std::vector<Talent> A list of affordable talents.
   */
  std::vector<Talent> getAffordableTalents() const;

  /**
   * @brief Get all unlocked talents.
   *
   * This function returns a list of all talents that have been unlocked.
   *
   * @return std::vector<std::string> A list of unlocked talents.
   */
  std::vector<std::string> getUnlockedTalents() const;

private:
  /**
   * @brief Check if a talent can be unlocked.
   *
   * This function checks if a specific talent can be unlocked, considering the price and the dependencies.
   *
   * @param talent The talent to be checked.
   * @return bool Returns true if the talent can be unlocked, false otherwise.
   */
  bool canUnlock(const Talent &talent) const;

  /**
   * @brief Applies the effects of a talent.
   *
   * This function applies the effects of the given talent by updating the appropriate game parameters.
   * The parameters are updated based on the talent name.
   * The function handles a set of predefined talents such as "speed", "adhd", "health", "salad", "haste", "spawnrate", and "stronk".
   * It's assumed that the talent has been checked to be unlocked before this function is called.
   *
   * @param talent A reference to the Talent object whose effects are to be applied.
   */
  void applyTalentEffects(const Talent &talent);

  /**
   * @brief Update the appearance of a talent button.
   *
   * This function updates the appearance of the button associated with a talent. It should be called when a talent has been unlocked.
   *
   * @param talent The talent whose button should be updated.
   */
  void updateButton(const Talent &talent);

  std::unordered_map<std::string, Talent> lockedTalents;
  std::unordered_set<std::string> unlockedTalents;

  int &crystals;
  int &wood;
  float &speedMultiplier;
  float &healthMultiplier;
  float &spawnRateMultiplier;
  float &hasteMultiplier;
  int &baseAttackDamage;
  Menu *renderMenu;

  int x, y;
};

#endif
