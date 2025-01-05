#pragma once
#include "SFML/Graphics.hpp"
#include "WorldGrid.h"
#include "TileSelector.h"
#include <sstream>

class UI
{
  sf::Font font;
  sf::Text gridInfoText;
  sf::Text uiText;

public:
  bool visible = true;

public:
  void Initialize();
  void Update(WorldGrid& worldGrid, TileSelector tileSelector);
  void Render(sf::RenderWindow& window);
};

