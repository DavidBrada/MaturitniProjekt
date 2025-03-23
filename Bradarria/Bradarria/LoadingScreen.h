#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

class LoadingScreen
{
public:
  sf::Sprite sprite;
  sf::Texture loadingScreenTexture;

  LoadingScreen()
  {
    if (loadingScreenTexture.loadFromFile("assets/textures/LoadingScreen.png"))
    {
      sprite.setTexture(loadingScreenTexture);

      std::cout << "Successfully loaded loading screen" << std::endl;
    }
    else
    {
      std::cout << "Failed to load loading screen" << std::endl;
    }
  }
};