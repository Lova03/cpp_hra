#include "Game.h"
#include "MenuScene.h"
#include "LevelSelectScene.h"
#include "LevelScene.h"
#include <memory>
#include <iostream>
#include <SDL2/SDL_image.h>
#include "utils.h"

using namespace std;

GameState Game::currentState = MENU;
SDL_Renderer *Game::renderer = nullptr;
SDL_Window *Game::window = nullptr;
bool Game::isRunning = false;
ResourceManager Game::resourceManager;
Save Game::save = Save("./examples/save.txt");
std::vector<std::pair<std::string, std::string>> Game::levels = {};
std::vector<LevelState> levelStates = {};
std::vector<std::pair<std::string, std::pair<int, int>>> Game::talents = {};
std::unique_ptr<MenuScene> Game::mainMenuScene = nullptr;
std::unique_ptr<LevelSelectScene> Game::levelSelectScene = nullptr;
std::unique_ptr<LevelScene> Game::currentLevelScene = nullptr;

void Game::changeState(GameState newState, const std::pair<std::string, std::string> &levelData)
{
  resetCursor();
  currentState = newState;

  if (newState == GameState::LEVEL)
  {
    currentLevelScene = std::make_unique<LevelScene>(levelData);
  }
}

void Game::setCursor(SDL_Cursor *cursor)
{
  SDL_SetCursor(cursor);
}

void Game::resetCursor()
{
  SDL_SetCursor(SDL_GetDefaultCursor());
}

bool Game::init(const char *title, int xpos, int ypos, int width, int height, bool fullscreen)
{
  int flags = 0;
  if (fullscreen)
    flags = SDL_WINDOW_FULLSCREEN;

  if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
  {
    window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
    if (window)
    {
      renderer = SDL_CreateRenderer(window, -1, 0);
      if (renderer)
      {
        SDL_SetRenderDrawColor(renderer, 5, 25, 35, 255);
        isRunning = true;
      }
      else
        return false; // Renderer not created
    }
    else
      return false; // window not created
  }
  else
    return false; // SDL did not init

  // SDL_image
  int imgFlags = IMG_INIT_PNG;
  if (!(IMG_Init(imgFlags) & imgFlags))
  {
    return false;
  }

  // SDL_ttf
  if (TTF_Init() == -1)
  {
    return false;
  }

  loadGameConfig("./examples/config.txt");

  save.init();

  mainMenuScene = make_unique<MenuScene>();
  levelSelectScene = make_unique<LevelSelectScene>();

  return true;
}

void Game::handleEvents()
{
  SDL_Event event;
  while (SDL_PollEvent(&event))
  {
    switch (currentState)
    {
    case MENU:
      mainMenuScene->handleInput(event);
      break;
    case LEVEL_SELECT:
      levelSelectScene->handleInput(event);
      break;
    default:
      currentLevelScene->handleInput(event);
      break;
    }
  }
}

void Game::update()
{
  switch (currentState)
  {
  case MENU:
    mainMenuScene->update();
    break;
  case LEVEL_SELECT:
    levelSelectScene->update();
    break;
  case LEVEL:
    currentLevelScene->update();
    break;
  default:
    mainMenuScene->update();
    break;
  }
}

void Game::render()
{
  SDL_RenderClear(renderer);
  switch (currentState)
  {
  case MENU:
    mainMenuScene->render();
    break;
  case LEVEL_SELECT:
    levelSelectScene->render();
    break;
  case LEVEL:
    currentLevelScene->render();
    break;
  default:
    mainMenuScene->render();
    break;
  }
  SDL_RenderPresent(renderer);
}

void Game::cleanup()
{

  resourceManager.freeAllResources();

  SDL_DestroyWindow(window);
  SDL_DestroyRenderer(renderer);
  SDL_Quit();
}

void Game::run()
{
  while (isRunning)
  {
    handleEvents();
    update();
    render();
  }
  cleanup();
}