#pragma once
#include <SFML/Graphics.hpp>

struct Settings
{
  bool instaBreak; // Toggles if blocks should be mined instantly or not
  int fpsLimit;
  int windowWidth = 1920;
  int windowHeight = 1080;

  Settings()
  {
    instaBreak = false;
    fpsLimit = 60;

    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
  }
};

