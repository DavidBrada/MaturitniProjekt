#pragma once

class InventoryCell
{
public:
  int id;
  sf::RectangleShape renderBody; // This object is renderd
  sf::RectangleShape logicBody; // This object detects mouse position
  bool isEmpty;
};