#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include "SceneManager.h"
#include "WorldGrid.h"

class WorldSettings
{
public:
  sf::String playerInput;
  sf::Text playerText;

  sf::Font font;

  sf::Texture bgTexture;
  sf::Sprite bgSprite;

  void Update(sf::RenderWindow& window, SceneManager& sceneManager);

  WorldSettings()
  {
    font.loadFromFile("assets/fonts/SegUIVar.ttf");
    playerText.setCharacterSize(80);
    playerText.setFont(font);
    playerText.setPosition(sf::Vector2f(500.f, 400.f));
    playerText.setString("Seed: ");

    if (bgTexture.loadFromFile("assets/textures/MainMenuBG.png"))
    {
      bgSprite.setTexture(bgTexture);

      std::cout << "Successfully loaded world settings BG" << std::endl;
    }
    else
    {
      std::cout << "Failed to load world settings BG" << std::endl;
    }
  }
};

