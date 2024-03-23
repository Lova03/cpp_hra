#include "ResourceManager.h"
#include "Game.h"
#include <iostream>

void ResourceManager::freeAllResources()
{
  // Free textures
  for (auto &texturePair : textureMap)
  {
    SDL_DestroyTexture(texturePair.second);
  }
  textureMap.clear();

  // Free fonts
  for (auto &fontPair : fonts)
  {
    TTF_CloseFont(fontPair.second);
  }
  fonts.clear();
}

SDL_Texture *ResourceManager::loadTexture(const std::string &path)
{
  auto it = textureMap.find(path);
  if (it != textureMap.end())
  {
    return it->second;
  }

  SDL_Texture *texture = IMG_LoadTexture(Game::renderer, path.c_str());
  if (texture == nullptr)
  {
    return nullptr;
  }

  textureMap[path] = texture;

  return texture;
}

void ResourceManager::freeTexture(const std::string &path)
{
  auto it = textureMap.find(path);
  if (it != textureMap.end())
  {
    SDL_DestroyTexture(it->second);
    textureMap.erase(it);
  }
}

TTF_Font *ResourceManager::loadFont(const std::string &fontPath, int fontSize)
{
  std::string key = fontPath + "_" + std::to_string(fontSize);
  if (fonts.count(key) == 0)
  {
    TTF_Font *font = TTF_OpenFont(fontPath.c_str(), fontSize);
    if (font == nullptr)
    {
      return nullptr;
    }
    fonts[key] = font;
  }
  return fonts[key];
}

void ResourceManager::freeFont(const std::string &fontPath, int fontSize)
{
  std::string key = fontPath + "_" + std::to_string(fontSize);
  if (fonts.count(key) > 0)
  {
    TTF_CloseFont(fonts[key]);
    fonts.erase(key);
  }
}