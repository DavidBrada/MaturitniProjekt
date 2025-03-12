#pragma once
#include <iostream>
#include "SFML/Graphics.hpp"
#include "Tile.h"
#include "AtlasTile.h"
#include "FastNoiseLite.h"

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
  bool worldLoaded; // This is probably not necessary

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

  void Initialize();
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

  std::string blocks[13] = {"air", "dirt", "grass", "dirt background", "stone", "iron", "Tree trunk", "Leaves", "Workbench", "Coal", "Tree bottom", "Branch", "Branch trunk"}; // Hardcoded block count, change later (I won't) 

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
    branch,
    branchTrunk
  };
};