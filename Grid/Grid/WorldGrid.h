#pragma once
#include "SFML/Graphics.hpp"
#include "Tile.h"

struct WorldGrid
{
  sf::Vector2u mousePosGrid;
  sf::Vector2f mousePosView;
  sf::Vector2i mousePosWindow;

  std::vector<std::vector<Tile>> tileMap;

  float tileSize = 16.f;
  unsigned tileSizeU = static_cast<unsigned>(tileSize);

  bool hasCollision = false;

  const int mapWidth = 200;
  const int mapHeight = 100;

  int groundLevel = 40;

  // Used for culling (rendering optimalization)
  int fromX = 0;
  int toX = 0;
  int fromY = 0;
  int toY = 0;

  void PlaceTile(int type, int xPos, int yPos);

  void Initialize();
  void Update(sf::RenderWindow& window);
  void Render(sf::RenderWindow& window, sf::View& view);

  std::string blocks[3] = {"air", "grass", "dirt"}; // Hardcoded block count, change later

  enum blockTypes
  {
    air,
    grass,
    dirt
  };
};

