#include "Text.h"
#include "Game.h"
#include <SDL2/SDL_ttf.h>

Text::Text(std::string text, std::string fontPath, int fontSize, SDL_Color color, int x, int y)
    : text(text), color(color), fontPath(fontPath), fontSize(fontSize)
{
  TTF_Font *font = Game::resourceManager.loadFont(fontPath, fontSize);
  SDL_Surface *surface = TTF_RenderText_Solid(font, text.c_str(), color);
  texture = SDL_CreateTextureFromSurface(Game::renderer, surface);
  SDL_FreeSurface(surface);
  Game::resourceManager.freeFont(fontPath, fontSize);

  rect.x = x;
  rect.y = y;
  SDL_QueryTexture(texture, nullptr, nullptr, &rect.w, &rect.h);
}

void Text::setText(std::string newText)
{
  text = newText;

  TTF_Font *font = Game::resourceManager.loadFont(fontPath, fontSize);
  SDL_Surface *surface = TTF_RenderText_Solid(font, text.c_str(), color);

  SDL_DestroyTexture(texture);

  texture = SDL_CreateTextureFromSurface(Game::renderer, surface);

  SDL_FreeSurface(surface);

  Game::resourceManager.freeFont(fontPath, fontSize);

  SDL_QueryTexture(texture, nullptr, nullptr, &rect.w, &rect.h);
}

SDL_Rect Text::getDimensions() const
{
  TTF_Font *font = Game::resourceManager.loadFont(fontPath, fontSize);
  SDL_Rect rect;
  if (font != nullptr)
  {
    TTF_SizeText(font, text.c_str(), &rect.w, &rect.h);
  }
  return rect;
}

void Text::setPosition(int x, int y)
{
  rect.x = x;
  rect.y = y;
}

Text::~Text()
{
  SDL_DestroyTexture(texture);
}

void Text::render()
{
  SDL_RenderCopy(Game::renderer, texture, nullptr, &rect);
}