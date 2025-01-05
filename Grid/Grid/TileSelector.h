#pragma once
#include "WorldGrid.h"

#include "SFML/Graphics.hpp"
struct TileSelector
{
  sf::RectangleShape selectorBody;
  sf::Vector2f selectorPosition;

  int selectedType = 0;

  void Initialize(WorldGrid& worldGrid);
  void Update(WorldGrid& worldGrid);
};