#include "Image.h"
#include "Game.h"

Image::Image(const std::string &imagePath, int x, int y)
{
  texture = Game::resourceManager.loadTexture(imagePath);

  int w, h;
  SDL_QueryTexture(texture, NULL, NULL, &w, &h);
  position = {x, y, w, h};
}

Image::~Image() = default;

void Image::render()
{
  SDL_RenderCopy(Game::renderer, texture, NULL, &position);
}

void Image::setPosition(int x, int y)
{
  position.x = x;
  position.y = y;
}