#pragma once
#include "SFML/Graphics.hpp"

struct Tile
{
  int type; // 0 - air; 1 - dirt; 2 - grass, etc.
  sf::RectangleShape shape;
  sf::Sprite sprite;
  sf::Vector2f position;
  bool hasCollision;
  bool mineable;
  float timeToMine;
};