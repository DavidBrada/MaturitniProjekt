#pragma once
#include <SFML/Graphics.hpp>

class World
{
public:
  Tile tile;

  void InitiateGrid(int xWindowSize, int yWindowSize);
  void Draw(sf::RenderWindow& window);
};

