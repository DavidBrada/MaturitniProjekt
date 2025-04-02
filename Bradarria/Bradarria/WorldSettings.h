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

  sf::RectangleShape startButton;
  sf::FloatRect startButtonHitBox;
  sf::Text startButtonText;
  sf::FloatRect startButtonTextHitBox;

  sf::RectangleShape mainBody;

  float buttonWidth = 500.f;
  float buttonHeight = 100.f;

  float transformStep;
  sf::Clock animationClock;

  sf::Vector2f mousePos;

  void Update(sf::RenderWindow& window, SceneManager& sceneManager);
  void Rneder(sf::RenderWindow& window);

  WorldSettings(sf::RenderWindow& window)
  {
    transformStep = 0.06f;

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

    float buttonCenterPos = (window.getPosition().x + window.getSize().x / 2) - buttonWidth / 2;

    mainBody.setPosition(sf::Vector2f(460.f, 190.f));
    mainBody.setSize(sf::Vector2f(1000.f, 700.f));
    mainBody.setFillColor(sf::Color(0, 0, 150, 200));

    startButton.setSize(sf::Vector2f(buttonWidth, buttonHeight));
    startButton.setPosition(sf::Vector2f(buttonCenterPos, 700.f));
    startButton.setFillColor(sf::Color::Yellow);

    startButtonText.setCharacterSize(100);
    startButtonText.setFont(font);
    startButtonText.setString("START");
    startButtonTextHitBox = startButtonText.getGlobalBounds();
    startButtonText.setOrigin(sf::Vector2f(startButtonText.getPosition().x + startButtonTextHitBox.width / 2, startButtonText.getPosition().y + startButtonTextHitBox.height / 2));
    startButtonText.setPosition(sf::Vector2f(startButton.getPosition().x + startButton.getSize().x / 2, startButton.getPosition().y + startButton.getSize().y / 4));

    startButtonText.setFillColor(sf::Color::Black);
    startButtonText.setOutlineColor(sf::Color::White);

    startButtonHitBox = startButton.getGlobalBounds();
  }
};

