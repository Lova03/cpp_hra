#include "ImageButton.h"
#include "Game.h"

ImageButton::ImageButton(int x, int y, int width, int height, const std::string &normalImagePath, const std::string &hoverImagePath, std::function<void()> onClick)
    : Button(x, y, width, height, onClick)
{
  normalTexture = Game::resourceManager.loadTexture(normalImagePath);
  hoverTexture = Game::resourceManager.loadTexture(hoverImagePath);
  currentTexture = normalTexture;
}

ImageButton::~ImageButton()
{
  if (handCursor != nullptr)
  {
    SDL_FreeCursor(handCursor);
  }
}

void ImageButton::update()
{
  int mouseX, mouseY;
  SDL_GetMouseState(&mouseX, &mouseY);

  if (isHovered(mouseX, mouseY))
  {
    currentTexture = hoverTexture;
    Game::setCursor(handCursor);
    wasHovered = true;
  }
  else if (wasHovered)
  {
    currentTexture = normalTexture;
    Game::resetCursor();
    wasHovered = false;
  }
}

void ImageButton::setHoverTexture(const std::string &path)
{
  hoverTexture = Game::resourceManager.loadTexture(path);
}

void ImageButton::setCurrentTexture(const std::string &path)
{
  currentTexture = Game::resourceManager.loadTexture(path);
}
void ImageButton::setNormalTexture(const std::string &path)
{
  normalTexture = Game::resourceManager.loadTexture(path);
}

void ImageButton::render()
{
  SDL_RenderCopy(Game::renderer, currentTexture, NULL, &rect);
}