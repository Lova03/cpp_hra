#include "Game.h"
#include "GameObject.h"
#include <string>

/**
 * @brief Constructor for the GameObject class.
 *
 * This constructor initializes the dimensions of the object and sets its texture to null.
 *
 * @param x The x-coordinate of the object.
 * @param y The y-coordinate of the object.
 * @param width The width of the object.
 * @param height The height of the object.
 */
GameObject::GameObject(int x, int y, int width, int height)
{
  objectRect.x = x;
  objectRect.y = y;
  objectRect.w = width;
  objectRect.h = height;

  texture = nullptr;
}

void GameObject::render()
{
  if (texture)
  {
    SDL_RenderCopy(Game::renderer, texture, NULL, &objectRect);
  }
}

void GameObject::update()
{
}

void GameObject::setPosition(int x, int y)
{
  objectRect.x = x;
  objectRect.y = y;
}

std::pair<int, int> GameObject::getPosition() const
{
  return {objectRect.x, objectRect.y};
}

void GameObject::setSize(int width, int height)
{
  objectRect.w = width;
  objectRect.h = height;
}

std::pair<int, int> GameObject::getSize() const
{
  return {objectRect.w, objectRect.h};
}

// AABB collision detection using SDL_Rect's and SDL's built in function
bool GameObject::checkCollision(const GameObject &other) const
{
  return SDL_HasIntersection(&objectRect, &other.objectRect);
}

void GameObject::setTexture(const std::string &filePath)
{
  texture = Game::resourceManager.loadTexture(filePath);
}
