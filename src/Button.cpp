#include "Button.h"

Button::Button(int x, int y, int width, int height, std::function<void()> onClick) : rect{x, y, width, height}, onClick(onClick), wasHovered(false), hoverable(true)
{
  handCursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND);
}

bool Button::isHovered(int mouseX, int mouseY)
{

  if (!hoverable)
    return false;

  return (mouseX >= rect.x && mouseX <= (rect.x + rect.w) && mouseY >= rect.y && mouseY <= (rect.y + rect.h));
}

void Button::setHoverable(bool val)
{
  hoverable = val;
}

void Button::setOnClick(std::function<void()> newOnClick)
{
  onClick = newOnClick;
}

void Button::handleMouseClick()
{
  onClick();
}