#include "MenuScene.h"
#include "Game.h"
#include "Text.h"
#include "TextButton.h"
#include <functional>
#include <memory>
#include <iostream>

MenuScene::MenuScene()
{

  Game::resetCursor();

  std::function<void()> startGameAction = []()
  {
    Game::changeState(GameState::LEVEL_SELECT);
  };

  std::function<void()> quitAction = []()
  {
    Game::isRunning = false;
  };

  SDL_Texture *tilesetTexture = Game::resourceManager.loadTexture("./assets/grass_tileset_16x16.png");

  int windowWidth, windowHeight;
  SDL_GetWindowSize(Game::window, &windowWidth, &windowHeight);

  int spacing = 10;

  mainMenu = std::make_unique<Menu>();
  mainMenu->createBackground(tilesetTexture, windowWidth, windowHeight);

  mainMenu->addImage("./assets/red_castle.png", 32, 88 + 32);
  mainMenu->addImage("./assets/red_soldier.png", 88, 88 + 80 + 34);
  mainMenu->addImage("./assets/red_soldier.png", 78, 88 + 80 + 50);
  mainMenu->addImage("./assets/red_soldier.png", 35, 88 + 80 + 14);
  mainMenu->addImage("./assets/red_soldier.png", 50, 88 + 80 + 20);

  mainMenu->addImage("./assets/green_castle.png", 50 + 576, 88 + 144);
  mainMenu->addImage("./assets/green_soldier.png", 35 + 576, 88 + 144 + 48 + 66);
  mainMenu->addImage("./assets/green_soldier.png", 60 + 576, 88 + 144 + 48 + 22);
  mainMenu->addImage("./assets/green_soldier.png", 88 + 576, 88 + 144 + 48 + 19);
  mainMenu->addImage("./assets/green_soldier.png", 32 + 576, 88 + 144 + 48 + 35);
  mainMenu->addImage("./assets/green_soldier.png", 72 + 576, 88 + 144 + 48 + 55);

  mainMenu->addImage("./assets/blue_castle.png", 44 + 96, 88 + 352);
  mainMenu->addImage("./assets/blue_soldier.png", 72 + 96, 88 + 352 + 48 + 12);
  mainMenu->addImage("./assets/blue_soldier.png", 11 + 96, 88 + 352 + 48 + 22);
  mainMenu->addImage("./assets/blue_soldier.png", 68 + 96, 88 + 352 + 48 + 28);
  mainMenu->addImage("./assets/blue_soldier.png", 87 + 96, 88 + 352 + 48 + 34);

  for (int j = 0; j < 4; j++)
  {
    for (int i = 0; i < 10; i++)
    {
      mainMenu->addImage("./assets/wall.png", 16 * i + j * 144, 288 + j * 16);
    }
  }

  for (int j = 0; j < 2; j++)
  {
    for (int i = 0; i < 5; i++)
    {
      mainMenu->addImage("./assets/wall.png", 256 + j * 16, 16 * i + 64 * j);
    }
  }

  for (int j = 0; j < 2; j++)
  {
    for (int i = 0; i < 5; i++)
    {
      mainMenu->addImage("./assets/wall.png", 544 + j * 16, 456 + 16 * i + 64 * j);
    }
  }

  int buttonWidth = 200;
  int buttonHeight = 50;

  int x = (windowWidth - buttonWidth) / 2;
  int y = (windowHeight - buttonHeight) / 2;

  mainMenu->addImageButton(x, y, buttonWidth, buttonHeight, "./assets/start.png", "./assets/start_hovered.png", startGameAction);
  mainMenu->addImageButton(x, y + buttonHeight + spacing, buttonWidth, buttonHeight, "./assets/quit.png", "./assets/quit_hovered.png", quitAction);

  int titleWidth = 700;
  int titleHeight = 200;

  std::unique_ptr<TextButton> title = std::make_unique<TextButton>((windowWidth - titleWidth) / 2, 20, titleWidth, titleHeight, "Kingdom Clash", "./assets/go3v2.ttf", 92, SDL_Color{255, 255, 255, 255}, SDL_Color{204, 31, 31, 255}, SDL_Color{204, 31, 31, 255}, SDL_Color{0, 0, 0, 255}, SDL_Color{0, 0, 0, 255}, []() {});
  title->setHoverable(false);

  mainMenu->addTextButton(std::move(title));
}

MenuScene::~MenuScene() = default;

void MenuScene::handleInput(SDL_Event &event)
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
      mainMenu->handleClick(mouseX, mouseY);
    }
    break;
  case SDL_KEYDOWN:
    if (event.key.keysym.sym == SDLK_ESCAPE)
      Game::isRunning = false;
  default:
    break;
  }
}

void MenuScene::update()
{
  mainMenu->update();
}

void MenuScene::render()
{
  mainMenu->renderBackground();
  mainMenu->render();
}