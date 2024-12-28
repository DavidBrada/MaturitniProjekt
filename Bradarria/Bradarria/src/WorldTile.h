#pragma once
#include <SFML/Graphics.hpp>

struct WorldTile
{
  int id = -1; // -1 is set to make debugging easier incase I forget to set the id

  sf::Vector2i position;
  
  //sf::IntRect rect;
  //sf::Sprite sprite; // Objects don't need an inicialization, because the default constructor will be called


};