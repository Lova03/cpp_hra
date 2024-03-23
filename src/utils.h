#ifndef UTILS_H
#define UTILS_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <utility>
#include <string>
#include <vector>

/**
 * @brief Load a texture from a file.
 *
 * This function loads a texture from a given file and returns a pointer to the loaded texture.
 *
 * @param filename The path to the image file.
 * @return SDL_Texture* A pointer to the loaded texture, or nullptr if loading fails.
 */
SDL_Texture *loadTexture(const char *filename);

/**
 * @brief Generate a random integer within a specified range.
 *
 * This function generates a random integer between the given minimum and maximum values.
 *
 * @param min The minimum value of the range (inclusive).
 * @param max The maximum value of the range (inclusive).
 * @return int A random integer within the specified range.
 */
int randomInt(int min, int max);

/**
 * @brief Generate a random tile type.
 *
 * This function generates a random tile type based on probabilities.
 * It returns 0 for the most common type, 1 for a less common type, and 2 for the least common type.
 *
 * @return int The randomly generated tile type.
 */
int randomTileType();

/**
 * @brief Load the game configuration from a file.
 *
 * This function loads the game configuration from a specified file.
 * It reads the file line by line and parses the content to populate the game configuration.
 *
 * @param filePath The path to the game configuration file.
 */
void loadGameConfig(const std::string &filePath);

/**
 * @brief Get the paths to the textures for a soldier based on its ID.
 *
 * This function returns a pair of paths to the regular and selected textures for a soldier,
 * based on the soldier's ID. The ID is used to determine the texture variation.
 *
 * @param id The ID of the soldier.
 * @return std::pair<std::string, std::string> The paths to the soldier's regular and selected textures.
 */
std::pair<std::string, std::string> getSoldierTexturePath(int id);

/**
 * @brief Get the path to the texture for a castle based on its ID.
 *
 * This function returns the path to the texture for a castle, based on the castle's ID.
 * The ID is used to determine the texture variation.
 *
 * @param id The ID of the castle.
 * @return std::string The path to the castle's texture.
 */
std::string getCastleTexturePath(int id);

/**
 * @brief Get the paths to the textures for a worker based on its ID.
 *
 * This function returns a pair of paths to the regular and selected textures for a worker,
 * based on the worker's ID. The ID is used to determine the texture variation.
 *
 * @param id The ID of the worker.
 * @return std::pair<std::string, std::string> The paths to the worker's regular and selected textures.
 */
std::pair<std::string, std::string> getWorkerTexturePath(int id);

#endif