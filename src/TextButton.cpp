#include "TextButton.h"
#include "Game.h"
#include <iostream>
#include <utility>

TextButton::TextButton(int x, int y, int width, int height, const std::string &text, const std::string &fontPath, int fontSize, SDL_Color textColor, SDL_Color backgroundColor, SDL_Color hoveredBackgroundColor, SDL_Color borderColor, SDL_Color hoveredBorderColor, std::function<void()> onClick)
    : Button(x, y, width, height, onClick), backgroundColor(backgroundColor), hoveredBackgroundColor(hoveredBackgroundColor), borderColor(borderColor), hoveredBorderColor(hoveredBorderColor)
{
  textElement = std::make_unique<Text>(text, fontPath, fontSize, textColor, 0, 0);

  SDL_Rect textDimensions = textElement->getDimensions();

  int textX = x + (width - textDimensions.w) / 2;
  int textY = y + (height - textDimensions.h) / 2;

  currentBackground = backgroundColor;
  currentBorder = borderColor;

  textElement->setPosition(textX, textY);
}

TextButton::~TextButton()
{
  if (handCursor != nullptr)
  {
    SDL_FreeCursor(handCursor);
  }
}

void TextButton::update()
{
  int mouseX, mouseY;
  SDL_GetMouseState(&mouseX, &mouseY);

  if (isHovered(mouseX, mouseY))
  {
    currentBackground = hoveredBackgroundColor;
    currentBorder = hoveredBorderColor;
    Game::setCursor(handCursor);
    wasHovered = true;
  }
  else if (wasHovered)
  {
    currentBackground = backgroundColor;
    currentBorder = borderColor;
    Game::resetCursor();
    wasHovered = false;
  }
}

void TextButton::render()
{

  SDL_SetRenderDrawColor(Game::renderer, currentBorder.r, currentBorder.g, currentBorder.b, currentBorder.a);
  SDL_RenderFillRect(Game::renderer, &rect);

  SDL_Rect bgRect = {rect.x + 2, rect.y + 2, rect.w - 4, rect.h - 4};
  SDL_SetRenderDrawColor(Game::renderer, currentBackground.r, currentBackground.g, currentBackground.b, currentBackground.a);
  SDL_RenderFillRect(Game::renderer, &bgRect);

  textElement->render();
}
