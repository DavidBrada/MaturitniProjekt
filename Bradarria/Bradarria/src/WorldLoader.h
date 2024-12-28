#pragma once
#include <iostream>
#include "WorldData.h"

class WorldLoader
{
public:
  void Save(std::string fileName);
  void Load(std::string fileName, WorldData& worldData);
};

