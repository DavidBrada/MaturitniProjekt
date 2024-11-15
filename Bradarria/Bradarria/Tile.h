#pragma once
#include <SFML/Graphics.hpp>
#include "GameLogic.h"

class Tile
{
private:
  int width = 60;
  int height = 60;

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

