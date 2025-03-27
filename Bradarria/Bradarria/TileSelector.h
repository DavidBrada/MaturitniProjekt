#pragma once
#include "WorldGrid.h"
#include "SFML/Graphics.hpp"

class WorldGrid;

struct TileSelector
{
  sf::RectangleShape selectorBody;
  sf::Vector2f selectorPosition;
  sf::Vector2f clickPosition;
  sf::Clock mineClock;
  float timeToMine = 0.5f;

  int selectedType = 0;
  int minedType = 0;

  void Initialize(WorldGrid& worldGrid);
  void Update(WorldGrid& worldGrid);
  void GetClickPos(WorldGrid& worldGrid);
  ~TileSelector();
};