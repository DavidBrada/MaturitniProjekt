#pragma once
#include <iostream>
#include "SFML/Graphics.hpp"
#include "Tile.h"
#include "Player.h"
#include "AtlasTile.h"
#include "FastNoiseLite.h"
#include "TileSelector.h"
#include "Inventory.h"
#include "CraftingMenu.h"

class Player;
class Inventory;
class TileSelector;
class Settings;
class CraftingMenu;

struct WorldGrid
{
  sf::Vector2u mousePosGrid;
  sf::Vector2f mousePosView;
  sf::Vector2i mousePosWindow;

  std::vector<std::vector<Tile>> tileMap;
  AtlasTile* atlasTiles;
  sf::Vector2i playerSpawnPos;

  float tileSize = 16.f;
  unsigned tileSizeU = static_cast<unsigned>(tileSize);

  static const int mapWidth = 500; // made static because of that stupid array two lines below
  const int mapHeight = 400;
  int terrainHeightValues[mapWidth];
  int worldSeed;
  int seedOffset; // Seed gets offset by a random value for each block

  sf::Texture tileAtlasTexture; // The whole tile atlas
  int xTileCount;
  int yTileCount;
  int tileCount;

  int groundLevel = 200; // Idk why this is initialized here
  int terrainHeight;

  // Used for culling (rendering optimalization)
  int fromX = 0;
  int toX = 0;
  int fromY = 0;
  int toY = 0;

  void PlaceTile(int type, int xPos, int yPos, std::vector<std::vector<Tile>>& worldMap);
  void MineTile(Player* player, Settings& settings, TileSelector* tileSelector, Inventory* inventory, CraftingMenu* craftingMenu);

  void Load();
  void Update(sf::RenderWindow& window);
  void Render(sf::RenderWindow& window, sf::View& view);

  void SetSprite(int type, float xPos, float yPos, std::vector<std::vector<Tile>>& worldMap);

  void InitializeCave();
  void GenerateTerrain();
  void GenerateTunnels();
  void SmoothCave(std::vector<std::vector<Tile>>& worldMap);
  void FillTiles();
  void PlaceGrass();

  void GenerateStone();
  void GenerateIron();
  void GenerateCoal();
  void GenerateTrees();
  void PlaceTree(int x, int yGround);

  std::string blocks[17] = { "air", "dirt", "grass", "dirt background", "stone", "iron", "Tree trunk", "Leaves", "Workbench", "Coal", "Tree bottom", "Branch trunk right", "Branch_1", "Branch_2", "Tree log", "Wooden plank", "Branch trunk left" };

  enum blockTypes
  {
    air,
    dirt,
    grass,
    dirtBackground,
    stone,
    iron,
    treeTrunk,
    leaves,
    workbench,
    coal,
    treeBottom,
    branchTrunkRight,
    branch1,
    branch2,
    treeLog,
    woodenPlank,
    branchTrunkLeft
  };

  WorldGrid()
  {
    tileMap.resize(mapWidth, std::vector<Tile>());

    if (tileAtlasTexture.loadFromFile("assets/textures/texture_test.png"))
    {
      xTileCount = tileAtlasTexture.getSize().x / tileSize;
      yTileCount = tileAtlasTexture.getSize().y / tileSize;

      std::cout << "Successfully loaded tile atlas." << std::endl;

      tileCount = xTileCount * yTileCount;

      atlasTiles = new AtlasTile[tileCount];

      // Tile atlas loop
      for (int x = 0; x < xTileCount; x++)
      {
        for (int y = 0; y < yTileCount; y++)
        {
          int index = x + y * xTileCount;

          atlasTiles[index].id = index;
          atlasTiles[index].position = sf::Vector2f(x * tileSize, y * tileSize);
        }
      }
    }
    else
    {
      std::cout << "Failed to load tilesheet." << std::endl;
    }
  }

  ~WorldGrid();
};