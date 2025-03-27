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

  static const int cellCount = 1;
  int craftableTypes[cellCount] = {8};

  int renderCellSize = 32;
  int logicCellSize;
  int gap = 16;
  int margin = 20;
  int marginTop = 300;

  bool inCrafting;

  int xFrom;
  int yFrom;
  int xLimit = 200;
  int yLimit = 100;

  int mousePosCrafting;
  int selectedPosition;

  void AddItem(int itemID, WorldGrid*& worldGrid);
  void Load(WorldGrid*& worldGrid);
  void Update(WorldGrid*& worldGrid, Inventory*& inventory);
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

