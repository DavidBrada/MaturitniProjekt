#pragma once
#include <SFML/Graphics.hpp>


struct Item
{
  sf::Sprite sprite;

  bool crafrable;
  std::vector<std::pair<int, int>> craftingRecepie; // First item ID and then the amount
};