#include "WorldLoader.h"
#include <fstream>
#include <string>
#include "WorldData.h"
#include "World.h"

void WorldLoader::Save(std::string fileName)
{
  std::ofstream file;

  file.open(fileName);
  file << "Output test." << std::endl;
  file << "0, 0, 1, 9, 7, 7" << std::endl;
  file.close();
}

void WorldLoader::Load(std::string fileName, WorldData& worldData)
{
  World world;

  std::string line;
  std::ifstream file(fileName);

  bool isValid = false;

  if (file.is_open())
  {
    while (std::getline(file, line))
    {
      if (!isValid)
      {
        if (line == "[World]")
        {
          isValid = true;
          continue;
        }
        else
        {
          std::cout << "Invalid world file." << std::endl;
          break;
        }
      }

      if(isValid)
      {
        try
        {
          int count = line.find('=');

          std::string variable = line.substr(0, count);
          std::string value = line.substr(count + 1, line.length() - count);

          //std::cout << "Variable: " << variable << std::endl;
          //std::cout << "Value: " << value << std::endl;

          if (variable == "tilesheet")
          {
            worldData.tilesheet = value;
          }
          else if (variable == "name")
          {
            worldData.name = value;
          }
          else if (variable == "worldWidth")
          {
            worldData.worldWidth = std::stoi(value);
          }
          else if (variable == "worldHeight")
          {
            worldData.worldHeight = std::stoi(value);
          }
          else if (variable == "tileWidth")
          {
            worldData.tileWidth = std::stoi(value);
          }
          else if (variable == "tileHeight")
          {
            worldData.tileHeight = std::stoi(value);
          }
          else if (variable == "xScale")
          {
            worldData.xScale = std::stoi(value);
          }
          else if (variable == "yScale")
          {
            worldData.yScale = std::stoi(value);
          }
          else if (variable == "dataLength")
          {
            worldData.dataLength = std::stoi(value);
          }
          else if (variable == "data")
          {
            worldData.data = new int[worldData.dataLength];

            int offset = 0;
            for (int i = 0; i < worldData.dataLength; i++)
            {
              int indexCount = value.find(',', offset);
              std::string worldIndex = value.substr(offset, indexCount - offset);
              std::cout << worldIndex << std::endl;
              worldData.data[i] = std::stoi(worldIndex);

              offset = indexCount + 1;
            }
          }
        }
        catch (const std::exception&)
        {
          std::cout << "World file'" << fileName << "' could not be loaded correctly." << std::endl;
        }
      }
    }
    
    std::cout << worldData.tilesheet << std::endl;
    std::cout << worldData.name << std::endl;
    std::cout << worldData.tileWidth << std::endl;
    std::cout << worldData.tileHeight << std::endl;
    std::cout << worldData.xScale << std::endl;
    std::cout << worldData.yScale << std::endl;
    std::cout << worldData.dataLength << std::endl;
    std::cout << worldData.data << std::endl;
    std::cout << worldData.worldWidth << std::endl;
    std::cout << worldData.worldHeight << std::endl;
    

    file.close();
  }
  else
  {
    std::cout << "Unable to open file. Path: " << fileName << std::endl;
  }
}
