#pragma once
#include "WorldGrid.h"

#include "SFML/Graphics.hpp"
struct TileSelector
{
  sf::RectangleShape selectorBody;
  sf::Vector2f selectorPosition;

  int selectedType = 0;

  int xCanPlaceFrom;
  int xCanPlaceTo;
  int yCanPlaceFrom;
  int yCanPlaceTo;

  bool canPlace; // If a block can be placed at current cursor position

  void Initialize(WorldGrid& worldGrid);
  void Update(WorldGrid& worldGrid);
};