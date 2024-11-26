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

  int mapNumbers[6] = {
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