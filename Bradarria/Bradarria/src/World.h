#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include "WorldTile.h"
#include "WorldLoader.h"
#include "WorldData.h"

class World
{
  sf::Texture tileSheetTexture;
  //WorldTile tiles[10];

  WorldTile* tiles;

  WorldLoader worldLoader;
  WorldData wd;

  int totalTiles;

  int totalTilesX;
  int totalTilesY;

  sf::Sprite* worldSprites;

public:
  World();
  ~World();

  void Initialize(std::string fileName);
  void Update(double deltaTime);
  void Draw(sf::RenderWindow& window);
};