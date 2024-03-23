#include "Game.h"
#include "TalentManager.h"
#include <functional>
#include <algorithm>
#include <cctype>

float TalentManager::getDefaultTalentValue(const std::string &talentName)
{
  if (talentName == "speed")
  {
    return 1.2;
  }
  else if (talentName == "haste")
  {
    return 0.9;
  }
  else if (talentName == "spawnrate")
  {
    return 0.9;
  }
  else if (talentName == "health")
  {
    return 1.2;
  }
  else if (talentName == "stronk")
  {
    return 12;
  }
  else if (talentName == "adhd")
  {
    return 1.35;
  }
  else if (talentName == "salad")
  {
    return 1.4;
  }

  return 1.0;
}

TalentManager::TalentManager(int &crystals, int &wood, float &speedMultiplier, float &healthMultiplier, float &spawnRateMultiplier, float &hasteMultiplier, int &baseAttackDamage, Menu *renderMenu)
    : crystals(crystals),
      wood(wood),
      speedMultiplier(speedMultiplier),
      healthMultiplier(healthMultiplier),
      spawnRateMultiplier(spawnRateMultiplier),
      hasteMultiplier(hasteMultiplier),
      baseAttackDamage(baseAttackDamage),
      renderMenu(renderMenu),
      x(80),
      y(250)
{
  for (const auto &talentPair : Game::talents)
  {
    const std::string &talentName = talentPair.first;
    const std::pair<int, int> &talentCost = talentPair.second;

    Talent talent;
    talent.name = talentName;
    talent.price = talentCost;
    talent.newValue = getDefaultTalentValue(talentName);
    registerTalent(talent);
  }
}

TalentManager::~TalentManager() = default;

void TalentManager::registerTalent(Talent &talent)
{
  const std::unordered_set<std::string> validTalents{"speed", "haste", "spawnrate", "health", "stronk"};
  if (validTalents.find(talent.name) == validTalents.end())
  {
    return;
  }

  lockedTalents.insert({talent.name, talent});

  std::function<void()> onClick = [this, talent]()
  {
    unlockTalent(talent.name);
  };

  if (talent.name == "speed")
  {
    Talent adhd;
    adhd.name = "adhd";
    adhd.price = std::make_pair(talent.price.first, talent.price.second);
    adhd.dependencies.push_back("speed");
    adhd.newValue = getDefaultTalentValue("adhd");
    if (renderMenu == nullptr)
      adhd.button = nullptr;
    lockedTalents.insert({adhd.name, adhd});
  }
  else if (talent.name == "health")
  {
    Talent salad;
    salad.name = "salad";
    salad.price = std::make_pair(talent.price.first, talent.price.second);
    salad.dependencies.push_back("health");
    salad.newValue = getDefaultTalentValue("salad");
    if (renderMenu == nullptr)
      salad.button = nullptr;
    lockedTalents.insert({salad.name, salad});
  }

  if (renderMenu != nullptr)
  {
    lockedTalents[talent.name].button = renderMenu->addImageButtonAndGet(x, y, 100, 100, "./assets/talent_" + talent.name + "_locked.png", "./assets/talent_" + talent.name + "_locked_hovered.png", onClick);
    renderMenu->addImage("./assets/crystals.png", x, y - 5 - 32 - 5 - 32);
    renderMenu->addText(std::to_string(lockedTalents[talent.name].price.first), "./assets/go3v2.ttf", 24, SDL_Color{255, 255, 255, 255}, x + 32 + 5, y - 5 - 32 - 5 - 28);
    renderMenu->addImage("./assets/planks.png", x, y - 5 - 32);
    renderMenu->addText(std::to_string(lockedTalents[talent.name].price.second), "./assets/go3v2.ttf", 24, SDL_Color{255, 255, 255, 255}, x + 32 + 5, y - 5 - 28);

    if (talent.name == "speed")
    {
      lockedTalents["adhd"].button = renderMenu->addImageButtonAndGet(x, y + 190, 100, 100, "./assets/talent_" + lockedTalents["adhd"].name + "_locked.png", "./assets/talent_" + lockedTalents["adhd"].name + "_locked_hovered.png", [this]()
                                                                      { unlockTalent(lockedTalents["adhd"].name); });

      renderMenu->addImage("./assets/crystals.png", x, y + 190 - 5 - 32 - 5 - 32);
      renderMenu->addText(std::to_string(lockedTalents["adhd"].price.first), "./assets/go3v2.ttf", 24, SDL_Color{255, 255, 255, 255}, x + 32 + 5, y + 190 - 5 - 32 - 5 - 28);
      renderMenu->addImage("./assets/planks.png", x, y + 190 - 5 - 32);
      renderMenu->addText(std::to_string(lockedTalents["adhd"].price.second), "./assets/go3v2.ttf", 24, SDL_Color{255, 255, 255, 255}, x + 32 + 5, y + 190 - 5 - 28);
    }

    if (talent.name == "health")
    {
      lockedTalents["salad"].button = renderMenu->addImageButtonAndGet(x, y + 190, 100, 100, "./assets/talent_" + lockedTalents["salad"].name + "_locked.png", "./assets/talent_" + lockedTalents["salad"].name + "_locked_hovered.png", [this]()
                                                                       { unlockTalent(lockedTalents["salad"].name); });

      renderMenu->addImage("./assets/crystals.png", x, y + 190 - 5 - 32 - 5 - 32);
      renderMenu->addText(std::to_string(lockedTalents["salad"].price.first), "./assets/go3v2.ttf", 24, SDL_Color{255, 255, 255, 255}, x + 32 + 5, y + 190 - 5 - 32 - 5 - 28);
      renderMenu->addImage("./assets/planks.png", x, y + 190 - 5 - 32);
      renderMenu->addText(std::to_string(lockedTalents["salad"].price.second), "./assets/go3v2.ttf", 24, SDL_Color{255, 255, 255, 255}, x + 32 + 5, y + 190 - 5 - 28);
    }
    x += 120;
  }
}

bool TalentManager::unlockTalent(const std::string &name)
{
  auto it = lockedTalents.find(name);
  if (it == lockedTalents.end())
  {
    printf("Failed to find talent that was supposed to be available. Game will stop running.\n");
    Game::isRunning = false;
    return false;
  }

  Talent &talent = it->second;
  if (!canUnlock(talent))
  {
    return false;
  }

  crystals -= talent.price.first;
  wood -= talent.price.second;

  unlockedTalents.insert(name);

  applyTalentEffects(talent);
  updateButton(talent);

  lockedTalents.erase(it);
  return true;
}

bool TalentManager::enableTalent(const std::string &name)
{

  std::string lowercaseTalentName = name;
  std::transform(lowercaseTalentName.begin(), lowercaseTalentName.end(), lowercaseTalentName.begin(), ::tolower);

  auto it = lockedTalents.find(lowercaseTalentName);
  if (it == lockedTalents.end())
  {
    return false;
  }

  Talent &talent = it->second;

  unlockedTalents.insert(lowercaseTalentName);

  applyTalentEffects(talent);
  updateButton(talent);

  lockedTalents.erase(it);
  return true;
}

bool TalentManager::canUnlock(const Talent &talent) const
{
  if (crystals < talent.price.first || wood < talent.price.second)
  {
    return false;
  }

  for (const auto &dependency : talent.dependencies)
  {
    if (unlockedTalents.find(dependency) == unlockedTalents.end())
    {
      return false;
    }
  }

  return true;
}

std::vector<Talent> TalentManager::getAffordableTalents() const
{
  std::vector<Talent> affordableTalents;
  for (const auto &talentPair : lockedTalents)
  {
    const Talent &talent = talentPair.second;
    if (canUnlock(talent))
    {
      affordableTalents.push_back(talent);
    }
  }
  return affordableTalents;
}

std::vector<std::string> TalentManager::getUnlockedTalents() const
{
  std::vector<std::string> talents(unlockedTalents.begin(), unlockedTalents.end());
  return talents;
}

void TalentManager::updateButton(const Talent &talent)
{
  if (renderMenu != nullptr)
  {

    talent.button->setOnClick([]() {});

    if (talent.name == "speed")
    {
      talent.button->setCurrentTexture("./assets/talent_speed.png");
      talent.button->setHoverTexture("./assets/talent_speed.png");
      talent.button->setNormalTexture("./assets/talent_speed.png");
    }
    else if (talent.name == "adhd")
    {
      talent.button->setCurrentTexture("./assets/talent_adhd.png");
      talent.button->setHoverTexture("./assets/talent_adhd.png");
      talent.button->setNormalTexture("./assets/talent_adhd.png");
    }
    else if (talent.name == "health")
    {
      talent.button->setCurrentTexture("./assets/talent_health.png");
      talent.button->setHoverTexture("./assets/talent_health.png");
      talent.button->setNormalTexture("./assets/talent_health.png");
    }
    else if (talent.name == "salad")
    {
      talent.button->setCurrentTexture("./assets/talent_salad.png");
      talent.button->setHoverTexture("./assets/talent_salad.png");
      talent.button->setNormalTexture("./assets/talent_salad.png");
    }
    else if (talent.name == "haste")
    {
      talent.button->setCurrentTexture("./assets/talent_haste.png");
      talent.button->setHoverTexture("./assets/talent_haste.png");
      talent.button->setNormalTexture("./assets/talent_haste.png");
    }
    else if (talent.name == "spawnrate")
    {
      talent.button->setCurrentTexture("./assets/talent_spawnrate.png");
      talent.button->setHoverTexture("./assets/talent_spawnrate.png");
      talent.button->setNormalTexture("./assets/talent_spawnrate.png");
    }
    else if (talent.name == "stronk")
    {
      talent.button->setCurrentTexture("./assets/talent_stronk.png");
      talent.button->setHoverTexture("./assets/talent_stronk.png");
      talent.button->setNormalTexture("./assets/talent_stronk.png");
    }
  }
}

void TalentManager::applyTalentEffects(const Talent &talent)
{
  if (talent.name == "speed" || talent.name == "adhd")
  {
    speedMultiplier = talent.newValue;
  }
  else if (talent.name == "health" || talent.name == "salad")
  {
    healthMultiplier = talent.newValue;
  }
  else if (talent.name == "haste")
  {
    hasteMultiplier = talent.newValue;
  }
  else if (talent.name == "spawnrate")
  {
    spawnRateMultiplier = talent.newValue;
  }
  else if (talent.name == "stronk")
  {
    baseAttackDamage = static_cast<int>(talent.newValue);
  }
}
