#include "MainMenu.h"

void MainMenu::Update(sf::RenderWindow& window, SceneManager& sceneManager)
{
  mousePos = sf::Vector2f(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y);
  sf::Event event;
  while (window.pollEvent(event))
  {
    switch (event.type)
    {
    case sf::Event::Closed:
      window.close();
      break;
    case sf::Event::KeyPressed:
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
      {
        window.close();
      }
      break;
    }
  }

  if (playButtonHitBox.contains(mousePos))
  {
    if (animationClock.getElapsedTime().asSeconds() >= 0.01f && playButtonText.getScale().x < 1.2f)
    {
      playButtonText.setScale(playButtonText.getScale().x + transformStep, playButtonText.getScale().y + transformStep);
      if (playButtonText.getOutlineThickness() < 6.f)
      {
        playButtonText.setOutlineThickness(playButtonText.getOutlineThickness() + 1.f);
      }
      animationClock.restart();
    }
    if (animationClock.getElapsedTime().asSeconds() >= 0.01f && exitButtonText.getScale().x > 1.f)
    {
      exitButtonText.setScale(exitButtonText.getScale().x - transformStep, exitButtonText.getScale().y - transformStep);
      if (exitButtonText.getOutlineThickness() > 0.f)
      {
        exitButtonText.setOutlineThickness(exitButtonText.getOutlineThickness() - 1.f);
      }
      animationClock.restart();
    }

    if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
    {
      sceneManager.currentScene = sceneManager.worldSettingsMenu;
    }
  }
  else if (exitButtonHitBox.contains(mousePos))
  {
    if (animationClock.getElapsedTime().asSeconds() >= 0.01f && exitButtonText.getScale().x < 1.2f)
    {
      exitButtonText.setScale(exitButtonText.getScale().x + transformStep, exitButtonText.getScale().y + transformStep);
      if (exitButtonText.getOutlineThickness() < 6.f)
      {
        exitButtonText.setOutlineThickness(exitButtonText.getOutlineThickness() + 1.f);
      }
      animationClock.restart();
    }
    if (animationClock.getElapsedTime().asSeconds() >= 0.01f && playButtonText.getScale().x > 1.f)
    {
      playButtonText.setScale(playButtonText.getScale().x - transformStep, playButtonText.getScale().y - transformStep);
      if (playButtonText.getOutlineThickness() > 0.f)
      {
        playButtonText.setOutlineThickness(playButtonText.getOutlineThickness() - 1.f);
      }
      animationClock.restart();
    }

    if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
    {
      window.close();
    }
  }
  else
  {
    if (animationClock.getElapsedTime().asSeconds() >= 0.01f && playButtonText.getScale().x > 1.f)
    {
      playButtonText.setScale(playButtonText.getScale().x - transformStep, playButtonText.getScale().y - transformStep);
      if (playButtonText.getOutlineThickness() > 0.f)
      {
        playButtonText.setOutlineThickness(playButtonText.getOutlineThickness() - 1.f);
      }
      animationClock.restart();
    }

    if (animationClock.getElapsedTime().asSeconds() >= 0.01f && exitButtonText.getScale().x > 1.f)
    {
      exitButtonText.setScale(exitButtonText.getScale().x - transformStep, exitButtonText.getScale().y - transformStep);
      if (exitButtonText.getOutlineThickness() > 0.f)
      {
        exitButtonText.setOutlineThickness(exitButtonText.getOutlineThickness() - 1.f);
      }
      animationClock.restart();
    }
  }
}

void MainMenu::Render(sf::RenderWindow& window)
{
  window.draw(bgSprite);
  //window.draw(playButton);
  window.draw(playButtonText);
  //window.draw(exitButton);
  window.draw(exitButtonText);
  window.draw(logoSprite);
}
