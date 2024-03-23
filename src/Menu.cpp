#include "Menu.h"
#include "Game.h"
#include "TextButton.h"
#include "ImageButton.h"
#include "Text.h"
#include "Image.h"
#include "utils.h"
#include <string>
#include <memory>
#include <utility>
#include <iostream>

Menu::Menu()
{
  background = nullptr;
  setBackgroundColor({0, 0, 0, 0});
}

Menu::~Menu()
{
  if (background)
  {
    SDL_DestroyTexture(background);
    background = nullptr;
  }
}

void Menu::update()
{
  for (auto &button : buttons)
  {
    button->update();
  }
}

void Menu::renderBackground()
{
  SDL_RenderCopy(Game::renderer, background, NULL, NULL);
}

void Menu::render()
{
  for (auto &image : images)
  {
    image->render();
  }

  for (auto &button : buttons)
  {
    button->render();
  }

  for (auto &text : texts)
  {
    text->render();
  }
}

void Menu::handleClick(int mouseX, int mouseY)
{
  for (auto &button : buttons)
  {
    if (button->isHovered(mouseX, mouseY))
    {
      button->handleMouseClick();
    }
  }
}

void Menu::addTextButton(int x, int y, int width, int height, const std::string &text, const std::string &fontPath, int fontSize, SDL_Color textColor, SDL_Color backgroundColor, SDL_Color hoveredBackgroundColor, SDL_Color borderColor, SDL_Color hoveredBorderColor, std::function<void()> onClick)
{
  buttons.push_back(std::make_unique<TextButton>(x, y, width, height, text, fontPath, fontSize, textColor, backgroundColor, hoveredBackgroundColor, borderColor, hoveredBorderColor, onClick));
}

void Menu::addImageButton(int x, int y, int width, int height, const std::string &normalImagePath, const std::string &hoveredImagePath, std::function<void()> onClick)
{
  buttons.push_back(std::make_unique<ImageButton>(x, y, width, height, normalImagePath, hoveredImagePath, onClick));
}

void Menu::addText(std::string text, std::string fontPath, int fontSize, SDL_Color color, int x, int y)
{
  texts.push_back(std::make_unique<Text>(text, fontPath, fontSize, color, x, y));
}

void Menu::addImage(const std::string &imagePath, int x, int y)
{
  images.push_back(std::make_unique<Image>(imagePath, x, y));
}

void Menu::addTextButton(std::unique_ptr<TextButton> button)
{
  buttons.push_back(std::move(button));
}

void Menu::addImageButton(std::unique_ptr<ImageButton> button)
{
  buttons.push_back(std::move(button));
}

void Menu::addText(std::unique_ptr<Text> text)
{
  texts.push_back(std::move(text));
}

void Menu::addImage(std::unique_ptr<Image> image)
{
  images.push_back(std::move(image));
}

Text *Menu::addTextAndGet(std::string text, std::string fontPath, int fontSize, SDL_Color color, int x, int y)
{
  texts.push_back(std::make_unique<Text>(text, fontPath, fontSize, color, x, y));
  return texts.back().get();
}

Text *Menu::addTextAndGet(std::unique_ptr<Text> text)
{
  texts.push_back(std::move(text));
  return texts.back().get();
}

ImageButton *Menu::addImageButtonAndGet(int x, int y, int width, int height, const std::string &normalImagePath, const std::string &hoveredImagePath, std::function<void()> onClick)
{
  auto newButton = std::make_unique<ImageButton>(x, y, width, height, normalImagePath, hoveredImagePath, onClick);
  imageButtons.push_back(newButton.get());
  buttons.push_back(std::move(newButton));
  return imageButtons.back();
}

void Menu::setBackground(const std::string &imagePath)
{
  SDL_Texture *texture = Game::resourceManager.loadTexture(imagePath);
  if (!texture)
  {
    std::cerr << "Error loading image: " << IMG_GetError() << std::endl;
    return;
  }
  background = texture;
}

void Menu::setBackgroundColor(SDL_Color color)
{
  int windowWidth, windowHeight;
  SDL_GetRendererOutputSize(Game::renderer, &windowWidth, &windowHeight);

  SDL_Texture *backgroundTexture = SDL_CreateTexture(Game::renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, windowWidth, windowHeight);

  if (backgroundTexture == nullptr)
    return;

  SDL_SetRenderTarget(Game::renderer, backgroundTexture);
  SDL_SetTextureBlendMode(backgroundTexture, SDL_BLENDMODE_BLEND);

  SDL_SetRenderDrawColor(Game::renderer, color.r, color.g, color.b, color.a);
  SDL_RenderClear(Game::renderer);

  SDL_SetTextureAlphaMod(backgroundTexture, color.a);

  SDL_SetRenderTarget(Game::renderer, NULL);

  if (background)
  {
    SDL_DestroyTexture(background);
  }
  background = backgroundTexture;
}

void Menu::createBackground(SDL_Texture *tilesetTexture, int windowWidth, int windowHeight)
{
  int tileWidth = 16;
  int tileHeight = 16;
  int tilesX = windowWidth / tileWidth + 1;
  int tilesY = windowHeight / tileHeight + 1;

  SDL_Texture *backgroundTexture = SDL_CreateTexture(Game::renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, windowWidth, windowHeight);
  if (backgroundTexture == nullptr)
    return;

  SDL_SetTextureBlendMode(backgroundTexture, SDL_BLENDMODE_BLEND);
  SDL_SetRenderTarget(Game::renderer, backgroundTexture);
  SDL_Rect destRect = {0, 0, tileWidth, tileHeight};

  for (int i = 0; i < tilesX; ++i)
  {
    for (int j = 0; j < tilesY; ++j)
    {
      destRect.x = i * tileWidth;
      destRect.y = j * tileHeight;

      int tileType = randomTileType();
      SDL_Rect srcRect = {0, tileType * tileHeight + 96, tileWidth, tileHeight};

      SDL_RenderCopy(Game::renderer, tilesetTexture, &srcRect, &destRect);
    }
  }
  SDL_SetRenderTarget(Game::renderer, NULL);
  if (background)
  {
    SDL_DestroyTexture(background);
  }
  background = backgroundTexture;
}