#include "Game.h"
#include "Player.h"
#include "utils.h"
#include <cmath>
#include <algorithm>
#include <string>
#include <functional>

Player::Player(int x, int y, int id, bool &talentsVisible, std::vector<std::unique_ptr<Unit>> &allUnits, std::vector<Unit *> &unitsToRemove, std::vector<std::unique_ptr<Wall>> &allWalls, std::vector<std::unique_ptr<Resource>> &allResources, std::vector<Castle *> &allCastles)
    : allUnits(allUnits),
      speedMultiplier(1.0),
      healthMultiplier(1.0),
      spawnRateMultiplier(1.0),
      hasteMultiplier(1.0),
      baseAttackDamage(8),
      baseAttackSpeed(3000),
      gatherRate(8000),
      wood(0),
      crystals(0),
      lastWood(0),
      lastCrystals(0),
      woodText(nullptr),
      crystalsText(nullptr),
      spawnInterval(15000),
      id(id),
      castle(x, y, 48, 48, id, allUnits, unitsToRemove, allWalls, allResources, allCastles, speedMultiplier, healthMultiplier, spawnRateMultiplier, hasteMultiplier, baseAttackDamage, baseAttackSpeed, gatherRate, wood, crystals, spawnInterval),
      isControlling(false),
      talentsVisible(talentsVisible)
{

  selectMenu = std::make_unique<Menu>();
  talentsMenu = std::make_unique<Menu>();
  talentManager = std::make_unique<TalentManager>(crystals, wood, speedMultiplier, healthMultiplier, spawnRateMultiplier, hasteMultiplier, baseAttackDamage, talentsMenu.get());

  talentsMenu->setBackgroundColor(SDL_Color{15, 15, 15, 210});

  talentsMenu->addImage("./assets/planks.png", 20, 10 + 50 + 30);
  woodText = talentsMenu->addTextAndGet("planks :  " + std::to_string(wood), "./assets/go3v2.ttf", 28, {255, 255, 255, 255}, 20 + 32 + 5, 10 + 50 + 35);

  talentsMenu->addImage("./assets/crystals.png", 20 + 320, 10 + 50 + 30);
  crystalsText = talentsMenu->addTextAndGet("crystals :  " + std::to_string(crystals), "./assets/go3v2.ttf", 28, {255, 255, 255, 255}, 20 + 32 + 5 + 320, 10 + 50 + 35);

  int windowWidth,
      windowHeight;
  SDL_GetWindowSize(Game::window, &windowWidth, &windowHeight);

  std::function<void()> deselectAll = [this]()
  {
    for (auto *unit : selectedUnits)
    {
      unit->setTexture(unit->getType() == "soldier" ? getSoldierTexturePath(unit->getOwnerId()).first : getWorkerTexturePath(unit->getOwnerId()).first);
    }
    selectedUnits.clear();
    Game::resetCursor();
    isControlling = false;
  };

  selectMenu->addImageButton(windowWidth - 20 - 50 - 50, 19, 50, 50, "./assets/deselect.png", "./assets/deselect_hovered.png", deselectAll);
}

Player::~Player() = default;

void Player::update()
{

  if (isControlling && !talentsVisible)
  {
    selectMenu->update();
  }

  if (talentsVisible)
  {
    talentsMenu->update();
  }

  if (woodText != nullptr && wood != lastWood)
  {
    lastWood = wood;
    woodText->setText("planks :  " + std::to_string(wood));
  }

  if (crystalsText != nullptr && crystals != lastCrystals)
  {
    lastCrystals = crystals;
    crystalsText->setText("crystals :  " + std::to_string(crystals));
  }
}
void Player::render()
{
  if (isControlling)
  {
    selectMenu->render();
  }
  if (talentsVisible)
  {
    talentsMenu->renderBackground();
    talentsMenu->render();
  }
}

void Player::handleInput(const SDL_Event &event)
{
  switch (event.type)
  {
  case SDL_MOUSEBUTTONDOWN:
    if (event.button.button == SDL_BUTTON_LEFT)
    {
      int mouseX = event.button.x;
      int mouseY = event.button.y;

      if (mouseY >= 88 && !talentsVisible)
      {
        startX = mouseX;
        startY = mouseY;
      }
    }
    break;
  case SDL_MOUSEBUTTONUP:
    if (event.button.button == SDL_BUTTON_LEFT)
    {
      int mouseX = event.button.x;
      int mouseY = event.button.y;

      if (talentsVisible)
      {
        talentsMenu->handleClick(mouseX, mouseY);
      }
      if (isControlling && !talentsVisible)
      {
        selectMenu->handleClick(mouseX, mouseY);
      }

      if (mouseY >= 88 && !talentsVisible)
      {
        endX = mouseX;
        endY = mouseY;

        if (isControlling)
        {
          setUnitsTarget(endX, endY);
          isControlling = false;
        }
        else
        {
          selectUnits();
          if (!selectedUnits.empty())
            isControlling = true;
        }
      }
    }
    break;
  default:
    break;
  }
}

void Player::selectUnits()
{
  int leftX = std::min(startX, endX);
  int rightX = std::max(startX, endX);
  int topY = std::min(startY, endY);
  int bottomY = std::max(startY, endY);

  for (auto &unit : allUnits)
  {
    int unitWidth = unit->getSize().first;
    int unitHeight = unit->getSize().second;

    if (unit->getOwnerId() == id &&
        unit->getActualX() >= leftX && (unit->getActualX() + unitWidth) <= rightX &&
        unit->getActualY() >= topY && (unit->getActualY() + unitHeight) <= bottomY)
    {
      unit->setTexture(unit->getType() == "soldier" ? getSoldierTexturePath(unit->getOwnerId()).second : getWorkerTexturePath(unit->getOwnerId()).second);
      selectedUnits.push_back(unit.get());
    }
  }
}

void Player::setUnitsTarget(int targetX, int targetY)
{
  for (auto *unit : selectedUnits)
  {
    unit->moveTo(targetX, targetY);
    unit->setTexture(unit->getType() == "soldier" ? getSoldierTexturePath(unit->getOwnerId()).first : getWorkerTexturePath(unit->getOwnerId()).first);
  }

  selectedUnits.clear();
}

std::vector<Unit *> &Player::getSelectedUnits()
{
  return selectedUnits;
}

Castle &Player::getCastle()
{
  return castle;
}

void Player::addUnit(int x, int y, const std::string &type, int health)
{
  castle.spawnUnit(x, y, type, health);
}

void Player::addCrystals(int amount) { crystals += amount; }
void Player::addWood(int amount) { wood += amount; }
TalentManager *Player::getTalentManager()
{
  return talentManager.get();
}

int Player::getCrystals() const { return crystals; }
int Player::getWood() const { return wood; }

void Player::setId(int id) { this->id = id; }
int Player::getId() const { return id; }