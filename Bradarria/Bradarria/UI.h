#pragma once
#include "SFML/Graphics.hpp"
#include "WorldGrid.h"
#include "TileSelector.h"
#include "Player.h"
#include "Inventory.h"
#include <sstream>

class UI
{
  sf::Font font;
  sf::Text gridInfoText;
  sf::Text uiText;
  sf::Text controlsText;
  sf::Text playerInfoText;

public:
  bool visible = true;
  float timeStep = 0.1f; // Time step of UI in seconds

public:
  void Initialize();
  void Update(WorldGrid& worldGrid, TileSelector& tileSelector, Player& player, Inventory& inventory);
  void Render(sf::RenderWindow& window);
};

