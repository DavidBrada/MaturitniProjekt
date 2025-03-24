#pragma once
#include <SFML/Graphics.hpp>

struct Item
{
  int id;
  sf::Sprite sprite;

  bool craftable;
  std::pair<int, int> craftingRecepie; // First item ID and then the amount

  Item(int _id, bool _craftable, int craftingMatID, int craftingMatAmount)
  {
    id = _id;
    craftable = _craftable;
    craftingRecepie.first = craftingMatID;
    craftingRecepie.second = craftingMatAmount;
  }
};