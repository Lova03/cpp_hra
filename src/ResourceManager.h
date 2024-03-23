#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <map>
#include <string>

/**
 * @class ResourceManager
 * @brief Manages the resources (textures and fonts) used in the game.
 *
 * The ResourceManager class is responsible for loading, storing, and freeing resources such as textures and fonts used in the game.
 * It provides methods to load and free textures and fonts, as well as managing their storage in internal containers.
 */
class ResourceManager
{
public:
  /**
   * @brief Free all the resources loaded in the manager.
   *
   * This function will destroy all textures and fonts, and clear their respective containers.
   */
  void freeAllResources();

  /**
   * @brief Load a texture from a file into the manager.
   *
   * If the texture is already loaded, it will return the existing texture. If not, it will load it from file.
   *
   * @param path Path to the texture file.
   * @return Pointer to the loaded SDL_Texture. nullptr if loading failed.
   */
  SDL_Texture *loadTexture(const std::string &path);

  /**
   * @brief Free a texture from the manager.
   *
   * If the texture is loaded in the manager, it will destroy the texture and remove it from the container.
   *
   * @param path Path to the texture file that was loaded previously.
   */
  void freeTexture(const std::string &path);

  /**
   * @brief Load a font from a file into the manager.
   *
   * If the font at the specific size is already loaded, it will return the existing font. If not, it will load it from file.
   *
   * @param fontPath Path to the font file.
   * @param fontSize The size of the font to be loaded.
   * @return Pointer to the loaded TTF_Font. nullptr if loading failed.
   */
  TTF_Font *loadFont(const std::string &fontPath, int fontSize);

  /**
   * @brief Free a font from the manager.
   *
   * If the font at the specific size is loaded in the manager, it will close the font and remove it from the container.
   *
   * @param fontPath Path to the font file that was loaded previously.
   * @param fontSize The size of the font that was loaded previously.
   */
  void freeFont(const std::string &fontPath, int fontSize);

private:
  std::map<std::string, SDL_Texture *> textureMap;
  std::map<std::string, TTF_Font *> fonts;
};
#endif