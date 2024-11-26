#pragma once
#include <SFML/Graphics.hpp>
#include "GameLogic.h"

class Tile
{
private:
  // 1 Tile has a side 18 pixels long and the tiles are scaled by 5, that's why it's 90
  int width = 90;
  int height = 90;

public:
  sf::Vector2f position{};
  sf::RectangleShape body{};

  int xPosition{ };
  int yPosition{ };

  void Initialize();
  void Update(sf::RenderWindow& window, sf::View& view, sf::Vector2i mousePosition);
  void Place(sf::Vector2f position);
  
  sf::FloatRect hitBox{};

  Tile(sf::Vector2f _position)
  {
    position = _position;
  }

  Tile();
};

