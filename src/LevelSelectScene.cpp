#include "LevelSelectScene.h"
#include "Game.h"
#include "Menu.h"
#include "Text.h"
#include "utils.h"
#include <functional>
#include <memory>
#include <iostream>

LevelSelectScene::LevelSelectScene()
{

  Game::resetCursor();

  std::function<void()> backAction = []()
  {
    Game::changeState(GameState::MENU);
  };

  std::function<void()> loadSave = []()
  {
    Game::save.load();
  };

  SDL_Texture *tilesetTexture = Game::resourceManager.loadTexture("./assets/grass_tileset_16x16.png");

  int windowWidth, windowHeight;
  SDL_GetWindowSize(Game::window, &windowWidth, &windowHeight);

  levelSelectMenu = std::make_unique<Menu>();
  levelSelectMenu->createBackground(tilesetTexture, windowWidth, windowHeight);

  int titleWidth = 700;
  int titleHeight = 100;
  std::unique_ptr<TextButton> title = std::make_unique<TextButton>((windowWidth - titleWidth) / 2, 20, titleWidth, titleHeight, "Level Selection", "./assets/go3v2.ttf", 72, SDL_Color{255, 255, 255, 255}, SDL_Color{204, 31, 31, 255}, SDL_Color{204, 31, 31, 255}, SDL_Color{0, 0, 0, 255}, SDL_Color{0, 0, 0, 255}, []() {});
  title->setHoverable(false);

  levelSelectMenu->addTextButton(std::move(title));

  levelSelectMenu->addTextButton(10, titleHeight + 20 + 20, 140, 50, "BACK", "./assets/empire.ttf", 24, {255, 255, 255, 255}, {0, 0, 0, 0}, {127, 127, 127, 255}, {255, 255, 255, 255}, {255, 255, 255, 255}, backAction);
  levelSelectMenu->addTextButton(10 + 140 + 10, titleHeight + 20 + 20, 240, 50, "LOAD SAVE", "./assets/empire.ttf", 24, {255, 255, 255, 255}, {0, 0, 0, 0}, {127, 127, 127, 255}, {255, 255, 255, 255}, {255, 255, 255, 255}, loadSave);

  int buttonY = titleHeight + 20 + 20 + 50 + 40;
  int buttonX = 90;

  for (const auto &levelData : Game::levels)
  {
    std::function<void()> levelAction = [levelData]()
    {
      Game::changeState(GameState::LEVEL, levelData);
    };

    levelSelectMenu->addTextButton(buttonX, buttonY, 300, 80, levelData.first, "./assets/empire.ttf", 40, {255, 255, 255, 255}, {0, 0, 255, 255}, {255, 0, 0, 255}, {0, 0, 0, 255}, {255, 255, 255, 255}, levelAction);

    buttonY += 90;
    if (buttonY > 520)
    {
      buttonY = titleHeight + 20 + 20 + 50 + 40;
      buttonX += 320;

      if (buttonX > 480)
        break;
    }
  }
}

LevelSelectScene::~LevelSelectScene() = default;

void LevelSelectScene::handleInput(SDL_Event &event)
{
  switch (event.type)
  {
  case SDL_QUIT:
    Game::isRunning = false;
    break;
  case SDL_MOUSEBUTTONUP:
    if (event.button.button == SDL_BUTTON_LEFT)
    {
      int mouseX = event.button.x;
      int mouseY = event.button.y;
      levelSelectMenu->handleClick(mouseX, mouseY);
    }
    break;
  case SDL_KEYDOWN:
    if (event.key.keysym.sym == SDLK_ESCAPE)
      Game::isRunning = false;
  default:
    break;
  }
}

void LevelSelectScene::update()
{
  levelSelectMenu->update();
}

void LevelSelectScene::render()
{
  levelSelectMenu->renderBackground();
  levelSelectMenu->render();
}