#include "Game.h"
#include "GameObject.h"
#include "Wall.h"

Wall::Wall(int x, int y, int width, int height)
    : GameObject(x, y, width, height)
{
  setTexture("./assets/wall.png");
}

Wall::~Wall() = default;

void Wall::render()
{
  if (texture)
  {
    SDL_RenderCopy(Game::renderer, texture, NULL, &objectRect);
  }
}

void Wall::update() {}