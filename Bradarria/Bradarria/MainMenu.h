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

  sf::Font mainFont;
  sf::Text playButtonText;

  sf::FloatRect playButtonTextHitBox;

  float transformStep;
  sf::Clock animationClock;

  sf::Vector2f mousePos;

  float buttonWidth = 500.f;
  float buttonHeight = 100.f;

  void Update(sf::RenderWindow& window, SceneManager& sceneManager);
  void Render(sf::RenderWindow& window);

  MainMenu(sf::RenderWindow& window)
  {
    transformStep = 0.06f;
    mainFont.loadFromFile("assets/fonts/SegUIVar.ttf");

    float buttonCenterPos = (window.getPosition().x + window.getSize().x / 2) - buttonWidth / 2;

    playButton.setSize(sf::Vector2f(buttonWidth, buttonHeight));
    playButton.setPosition(sf::Vector2f(buttonCenterPos, 200.f));
    playButton.setFillColor(sf::Color::Yellow);

    playButtonText.setCharacterSize(100);
    playButtonText.setFont(mainFont);
    playButtonText.setString("PLAY");
    playButtonTextHitBox = playButtonText.getGlobalBounds();
    playButtonText.setOrigin(sf::Vector2f(playButtonText.getPosition().x + playButtonTextHitBox.width / 2, playButtonText.getPosition().y + playButtonTextHitBox.height / 2));
    playButtonText.setPosition(sf::Vector2f(playButton.getPosition().x + playButton.getSize().x / 2, playButton.getPosition().y + playButton.getSize().y / 4));
    
    playButtonText.setFillColor(sf::Color::Black);
    playButtonText.setOutlineColor(sf::Color::White);


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

