#pragma once
#include "SFML/Graphics.hpp"

class Player
{
  int width = 36;
  int height = 72;

public:
  float moveSpeed = 100.f;

  sf::RectangleShape body;

  void Initialize();
  void Load();
  void Update();
  void Draw();
};