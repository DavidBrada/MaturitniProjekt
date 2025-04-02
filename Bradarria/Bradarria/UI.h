#pragma once
#include "SFML/Graphics.hpp"
#include "WorldGrid.h"
#include "TileSelector.h"
#include "Player.h"
#include "Inventory.h"
#include "CraftingMenu.h"
#include <sstream>

class UI
{
  sf::Font font;
  sf::Text gridInfoText;
  sf::Text uiText;
  sf::Text controlsText;
  sf::Text playerInfoText;
  sf::Text inventoryText;

public:
  bool visible = false;
  float timeStep = 0.1f; // Time step of UI in seconds, it's probably useless tho

public:
  void Initialize();
  void Update(WorldGrid& worldGrid, TileSelector& tileSelector, Player& player, Inventory& inventory, CraftingMenu& craftingMenu);
  void UpdateInventory(Inventory& inventory, sf::RenderWindow& window);
  void Render(sf::RenderWindow& window, Inventory& inventory);

  ~UI();
};