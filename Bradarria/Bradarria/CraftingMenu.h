#pragma once
#include "SFML/Graphics.hpp"
#include "Item.h"
#include "WorldGrid.h"
#include "CraftingCell.h"

class Inventory;

class CraftingMenu
{
public:
  std::vector<CraftingCell> craftingContainer;

  static const int cellCount = 2;
  int craftableTypes[cellCount] = {8, 15};

  // These are hard-coded because I don't wanna rewrite the entire program
  bool workbenchCrafrable = false;
  bool woodenPlankCrafrable = false;

  int renderCellSize = 32;
  int logicCellSize;
  int gap = 16;
  int margin = 20;
  int marginTop = 300;

  bool inCrafting;
  bool isAccessible;

  int xFrom;
  int yFrom;
  int xLimit = 200;
  int yLimit = 100;

  int mousePosCrafting;
  int selectedPosition;

  void Load(WorldGrid*& worldGrid);
  void Update(WorldGrid*& worldGrid, Inventory*& inventory, sf::RenderWindow& window);
  void GetClickPos();
  void SetSprite(int type, int pos, sf::Texture& tileAtlasTexture, AtlasTile*& atlasTiles);
  void Render(sf::RenderWindow& window);


  CraftingMenu()
  {
    logicCellSize = renderCellSize + gap;
    craftingContainer.resize(cellCount);
  }

  ~CraftingMenu();
};

