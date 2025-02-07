#pragma once
#include "WorldGrid.h"
#include "Player.h"

#include "SFML/Graphics.hpp"
struct TileSelector
{
  sf::RectangleShape selectorBody;
  sf::Vector2f selectorPosition;
  sf::Vector2f clickPosition;
  sf::Clock mineClock;

  int selectedType = 0;

  void Initialize(WorldGrid& worldGrid);
  void Update(WorldGrid& worldGrid);
  void GetClickPos(WorldGrid& worldGrid);
};