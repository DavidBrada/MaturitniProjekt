#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include "SceneManager.h"


struct MainMenu
{
  sf::RectangleShape playButton;
  sf::RectangleShape exitButton;

  sf::FloatRect playButtonHitBox;
  sf::FloatRect exitButtonHitBox;

  sf::Texture bgTexture;
  sf::Sprite bgSprite;

  sf::String playerInput;
  sf::Text playerText;

  bool isActive;
  sf::Vector2f mousePos;

  float buttonWidth = 500.f;
  float buttonHeight = 100.f;

  void Update(sf::RenderWindow& window, SceneManager& sceneManager);
  void Render(sf::RenderWindow& window);

  MainMenu(sf::RenderWindow& window)
  {
    isActive = true;

    float buttonCenterPos = (window.getPosition().x + window.getSize().x / 2) - buttonWidth / 2;

    playButton.setSize(sf::Vector2f(buttonWidth, buttonHeight));
    playButton.setPosition(sf::Vector2f(buttonCenterPos, 200.f));
    playButton.setFillColor(sf::Color::Yellow);

    exitButton.setSize(sf::Vector2f(buttonWidth, buttonHeight));
    exitButton.setPosition(sf::Vector2f(buttonCenterPos, 700.f));
    exitButton.setFillColor(sf::Color::Blue);

    playButtonHitBox = playButton.getGlobalBounds();
    exitButtonHitBox = exitButton.getGlobalBounds();


    if (bgTexture.loadFromFile("assets/textures/MainMenuBG.png"))
    {
      bgSprite.setTexture(bgTexture);

      std::cout << "Successfully loaded main menu BG" << std::endl;
    }
    else
    {
      std::cout << "Failed to load main menu BG" << std::endl;
    }
  }
};

