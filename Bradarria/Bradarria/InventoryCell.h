#pragma once

class InventoryCell
{
public:
  int id;
  sf::RectangleShape renderBody; // This object is renderd
  sf::RectangleShape logicBody; // This object detects mouse position

  sf::Sprite itemSprite;
  sf::Text text;

  bool isEmpty;
  int quantity; // How much of one block is stored in this cell
};