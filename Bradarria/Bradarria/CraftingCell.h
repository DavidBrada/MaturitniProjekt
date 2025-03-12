#pragma once
#include <SFML/Graphics.hpp>


struct CraftingCell
{
  int id;
  sf::RectangleShape renderBody; // This object is renderd
  sf::RectangleShape logicBody; // This object detects mouse position

  sf::Sprite itemSprite;
};