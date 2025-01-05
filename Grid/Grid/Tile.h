#pragma once
#include "SFML/Graphics.hpp"

struct Tile
{
  int type; // 0 - air; 1 - grass; 2 - dirt, etc.
  sf::RectangleShape shape;
  bool hasCollision;
};

