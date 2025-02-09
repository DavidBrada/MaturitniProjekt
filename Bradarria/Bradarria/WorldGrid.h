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

  static const int mapWidth = 500;
  const int mapHeight = 400;

  int terrainHeightValues[mapWidth];


  sf::Texture tileAtlasTexture;
  int xTileCount;
  int yTileCount;
  int tileCount;

  int groundLevel = 200;
  int terrainHeight;

  // Used for culling (rendering optimalization)
  int fromX = 0;
  int toX = 0;
  int fromY = 0;
  int toY = 0;

  void PlaceTile(int type, int xPos, int yPos, std::vector<std::vector<Tile>>& worldMap);

  void Initialize();
  void Update(sf::RenderWindow& window);
  void Render(sf::RenderWindow& window, sf::View& view);
  void InitializeCave();
  void GenerateTerrain();
  void GenerateTunnels();
  void SmoothCave(std::vector<std::vector<Tile>>& worldMap);
  void FillTiles();

  void GenerateStone();
  void GenerateIron();

  std::string blocks[6] = {"air", "dirt", "grass", "dirt background", "stone", "iron"}; // Hardcoded block count, change later

  enum blockTypes
  {
    air,
    dirt,
    grass,
    dirtBackground,
    stone,
    iron
  };
};