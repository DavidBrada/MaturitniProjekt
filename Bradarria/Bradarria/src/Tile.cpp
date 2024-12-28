#include "Tile.h"
#include <iostream>
#include <vector>

void Tile::Initialize()
{
  body.setSize(sf::Vector2f(width, height));
  body.setFillColor(sf::Color::White);

  hitBox.width = body.getSize().x;
  hitBox.height = body.getSize().y;

  hitBox.left = body.getOrigin().x;
  hitBox.top = body.getOrigin().y;
}

void Tile::Update(sf::RenderWindow& window, sf::View& view, sf::Vector2i mousePosition)
{
  mousePosition = sf::Mouse::getPosition(window);
  sf::Vector2f mouseCoord = window.mapPixelToCoords(mousePosition);
  
  // Calculate tile position based on mouse position ------------------ (Formula with explanation will be added to docs)
  xPosition = std::round((mouseCoord.x - width / 2) / width) * width;
  yPosition = std::round((mouseCoord.y - height / 2) / height) * height;
  position = sf::Vector2f(xPosition, yPosition);
  
  //std::cout << mouseCoord.x << std::endl;
  body.setPosition(position);
}

void Tile::Place(sf::Vector2f position)
{
  body.setPosition(position);
  Initialize();

  //std::cout << "Tile placed!" << std::endl;
}

Tile::Tile(){}
