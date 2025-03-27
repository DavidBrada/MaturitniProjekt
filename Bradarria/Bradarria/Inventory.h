#pragma once
#include <SFML/Graphics.hpp>
#include "AtlasTile.h"
#include "WorldGrid.h"
#include "InventoryCell.h"
#include "TileSelector.h"

class WorldGrid;

struct Inventory
{
  sf::Vector2i mousePosWindow;

  // Width and height in pixels
  int renderCellSize = 32; // Actually visible cells to make it look more terraria-like and more pretty
  int logicCellSize; // these cells react to mouse input and determine the position on the grid
  int gap = 16; // Gap between cells
  int margin = 20; // Margin of the whole inventory from the window border

  // Position relative to window. This determines if the player has cursor in the inventory
  int xLimit;
  int yLimit;


  // Cell count
  int xCellCount = 10;
  int yCellCount = 5;

  static const int inventorySize = 50;
  std::vector<std::vector<InventoryCell>> container;
  int storedItems[inventorySize];

  bool open = false;
  bool inInventory;
  sf::Vector2i mousePosInventory;
  sf::Vector2f selectedPosition;

  void Initialize();
  void Load();
  void Update(WorldGrid& worldGrid, sf::RenderWindow& window);
  void Render(sf::RenderWindow& window);
  void GetClickPos();
  void SetSprite(int type, float xPos, float yPos, sf::Texture& tileAtlasTexture, AtlasTile*& atlasTiles);
  void StoreItem(int minedType, WorldGrid& worldGrid);

  ~Inventory();
};

