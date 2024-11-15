#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "Tile.h"

class GameLogic
{
public:
  void InitializeSettings();
  
  float deltaTime{};

  int windowWidth = 1024;
  int windowHeight = 576;

  // Default size of main camera (how much of the world is visible)
  int xDefaultZoom = windowWidth * 2;
  int yDefaultZoom = windowHeight * 2;

  int zoomLevel = 1; // Keeps track of number of zoom level changes (bigger value for broader view)
  int maxZoomLevel = 3;
  int minZoomLevel = 0;

  // --------------- MOVE TO World.cpp once created --------------------------------
  // 

  sf::Vector2i mousePosition{};

  sf::ContextSettings settings;

};

