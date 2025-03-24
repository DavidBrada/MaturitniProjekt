#pragma once
#include <SFML/Graphics.hpp>


struct CraftingCell
{
  int id;
  int craftItemID;
  sf::RectangleShape renderBody; // This object is renderd
  sf::RectangleShape logicBody; // This object detects mouse position
  sf::FloatRect logicRect;

  sf::Sprite itemSprite;
};