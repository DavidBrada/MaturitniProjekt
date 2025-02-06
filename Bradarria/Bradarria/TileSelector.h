#pragma once
#include "WorldGrid.h"

#include "SFML/Graphics.hpp"
struct TileSelector
{
  sf::RectangleShape selectorBody;
  sf::Vector2f selectorPosition;
  sf::Vector2f clickPosition;
  sf::Clock mineClock;

  bool mining;

  int selectedType = 0;

  int xCanPlaceFrom;
  int xCanPlaceTo;
  int yCanPlaceFrom;
  int yCanPlaceTo;

  bool canPlace; // If a block can be placed at current cursor position

  void Initialize(WorldGrid& worldGrid);
  void Update(WorldGrid& worldGrid);
  void GetClickPos(WorldGrid& worldGrid);
};