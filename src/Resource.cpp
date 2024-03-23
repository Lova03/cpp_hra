#include "Resource.h"
#include "Game.h"
#include <string>

Resource::Resource(int x, int y, int width, int height, std::string type) : GameObject(x, y, width, height), type(type)
{
  if (type == "crystals")
  {
    setTexture("./assets/crystals_ore.png");
  }
  else
  {
    setTexture("./assets/wood_ore.png");
  }
}

Resource::~Resource() = default;

std::string Resource::getType() const { return type; };
void Resource::setType(std::string newType) { type = newType; };
