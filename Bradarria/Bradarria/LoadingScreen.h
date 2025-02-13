#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

class LoadingScreen
{
public:
  sf::Sprite sprite;
  sf::Texture loadingScreenTexture;

  void Initialize();
};

