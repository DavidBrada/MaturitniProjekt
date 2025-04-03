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

  sf::Texture logoTexture;
  sf::Sprite logoSprite;

  sf::Font mainFont;
  sf::Text playButtonText;
  sf::Text exitButtonText;

  sf::FloatRect playButtonTextHitBox;
  sf::FloatRect exitButtonTextHitBox;

  float transformStep;
  sf::Clock animationClock;

  sf::Vector2f mousePos;

  float buttonWidth = 600.f;
  float buttonHeight = 200.f;

  void Update(sf::RenderWindow& window, SceneManager& sceneManager);
  void Render(sf::RenderWindow& window);

  MainMenu(sf::RenderWindow& window)
  {
    transformStep = 0.06f;
    mainFont.loadFromFile("assets/fonts/PixelifySans-VariableFont_wght.ttf");

    float buttonCenterPos = (window.getPosition().x + window.getSize().x / 2) - buttonWidth / 2;

    playButton.setSize(sf::Vector2f(buttonWidth, buttonHeight));
    playButton.setPosition(sf::Vector2f(buttonCenterPos, 480.f));
    playButton.setFillColor(sf::Color::Yellow);

    playButtonText.setCharacterSize(120);
    playButtonText.setFont(mainFont);
    playButtonText.setString("PLAY");
    playButtonTextHitBox = playButtonText.getGlobalBounds();
    playButtonText.setOrigin(sf::Vector2f(playButtonText.getPosition().x + playButtonTextHitBox.width / 2, playButtonText.getPosition().y + playButtonTextHitBox.height / 2));
    playButtonText.setPosition(sf::Vector2f(playButton.getPosition().x + playButton.getSize().x / 2, playButton.getPosition().y + playButton.getSize().y / 4));
    
    playButtonText.setFillColor(sf::Color::Black);
    playButtonText.setOutlineColor(sf::Color::Yellow);


    exitButton.setSize(sf::Vector2f(buttonWidth, buttonHeight));
    exitButton.setPosition(sf::Vector2f(buttonCenterPos, 750.f));
    exitButton.setFillColor(sf::Color::Yellow);

    exitButtonText.setCharacterSize(80);
    exitButtonText.setFont(mainFont);
    exitButtonText.setString("QUIT");
    exitButtonTextHitBox = exitButtonText.getGlobalBounds();
    exitButtonText.setOrigin(sf::Vector2f(exitButtonText.getPosition().x + exitButtonTextHitBox.width / 2, exitButtonText.getPosition().y + exitButtonTextHitBox.height / 2));
    exitButtonText.setPosition(sf::Vector2f(exitButton.getPosition().x + exitButton.getSize().x / 2, exitButton.getPosition().y + exitButton.getSize().y / 4));

    exitButtonText.setFillColor(sf::Color::Black);
    exitButtonText.setOutlineColor(sf::Color::Yellow);

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

    if (logoTexture.loadFromFile("assets/textures/logo.png"))
    {
      logoSprite.setTexture(logoTexture);
      std::cout << "Successfully loaded logo" << std::endl;
    }
    else
    {
      std::cout << "Failed to load logo" << std::endl;
    }

    logoSprite.setPosition(sf::Vector2f(window.getSize().x / 2 - 400, 50.f));
  }
};