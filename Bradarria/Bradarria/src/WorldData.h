#pragma once
#include <string>

struct WorldData
{
  std::string tilesheet;
  std::string name;

  int worldWidth = 0; // How many tiles on X
  int worldHeight = 0; // How many tiles on Y

  int tileWidth = 0;
  int tileHeight = 0;

  int xScale = 0;
  int yScale = 0;

  int dataLength = 0;
  int* data = nullptr;
};