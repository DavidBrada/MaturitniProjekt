#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include "WorldTile.h"

class World
{
  sf::Texture tileSheetTexture;
  //WorldTile tiles[10];

  WorldTile* tiles;

  int totalTiles;

  int tileWidth;
  int tileHeight;

  int totalTilesX;
  int totalTilesY;

  int worldScale = 5;

   int worldWidth; // How many tiles on X
   int worldHeight; // How many tiles on Y
   static const int worldSize = 6; // Total tiles in the world

  int mapNumbers[worldSize] = {
    0, 0, 1,
    9, 9, 7

  };

  sf::Sprite mapSprites[6];

public:
  World();
  ~World();

  void Initialize();
  void Update(double deltaTime);
  void Draw(sf::RenderWindow& window);
};