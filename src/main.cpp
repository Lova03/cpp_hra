#include <iostream>
#include <SDL2/SDL.h>
#include "Game.h"

int main(int argc, char *argv[])
{
  Game game;

  // Game init
  if (game.init("Kingdom Clash", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, false))
  {
    game.run();
  }

  // End game
  game.cleanup();

  return EXIT_SUCCESS;
}