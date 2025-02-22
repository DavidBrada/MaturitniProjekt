#pragma once
#include <SFML/Graphics.hpp>
#include "AtlasTile.h"
#include "WorldGrid.h"
#include "InventoryCell.h"

class Inventory
{
  std::vector<std::vector<InventoryCell>> container;
  sf::Vector2i mousePosWindow;
  sf::Vector2i mousePosInventory;
  //AtlasTile* uiTextures;

  // Width and height in pixels
  int renderCellSize = 32;
  int logicCellSize;
  int gap = 16;
  int margin = 20;

  // Cell count
  int xCellCount = 10;
  int yCellCount = 5;

  // Position relative to window. This determines if the player has cursor in the inventory
  int xLimit;
  int yLimit;

public:
  bool open = false;
  bool inInventory;

  void Inicialize();
  void Load(sf::View view);
  void Update(sf::View& view, WorldGrid& worldGrid, sf::RenderWindow& window);
  void Render(sf::RenderWindow& window);
};

