#include "World.h"

World::World() : 
  totalTilesX(0), totalTilesY(0),
  totalTiles(0), tiles(nullptr), worldSprites(nullptr)
{
}

World::~World()
{
  delete[] worldSprites;
  delete[] tiles;
}

void World::Initialize(std::string fileName)
{
  // World loader

  worldLoader.Load(fileName, wd);

  worldSprites = new sf::Sprite[wd.dataLength];

  if (tileSheetTexture.loadFromFile(wd.tilesheet))
  {
    totalTilesX = tileSheetTexture.getSize().x / wd.tileWidth;
    totalTilesY = tileSheetTexture.getSize().y / wd.tileHeight;

    //std::cout << totalTilesX << std::endl;
    //std::cout << totalTilesY << std::endl;

    std::cout << "Tilesheet loaded." << std::endl;
    
    totalTiles = totalTilesX * totalTilesY;

    tiles = new WorldTile[totalTiles]; // 'new' keyword creates the array on heap (RAM) instead of stack (used for variables; has size of ~1MB). It's better when the array size is unknown


    for (int y = 0; y < totalTilesY; y++)
    {
      for (int x = 0; x < totalTilesX; x++)
      {
        int i = x + y * totalTilesX;

        tiles[i].id = i;
        tiles[i].position = sf::Vector2i(x * wd.tileWidth, y * wd.tileHeight);
      }
    }

  }
  else
  {
    std::cout << "Failed to load tilesheet." << std::endl;
  }


  for (int y = 0; y < wd.worldHeight; y++)
  {
    for (int x = 0; x < wd.worldWidth; x++)
    {
      int i = x + y * wd.worldWidth;

      int index = wd.data[i];

      worldSprites[i].setTexture(tileSheetTexture);
      worldSprites[i].setTextureRect(sf::IntRect(
        tiles[index].position.x,
        tiles[index].position.y,
        wd.tileWidth,
        wd.tileHeight
      ));
      worldSprites[i].setScale(sf::Vector2f(wd.xScale, wd.yScale));
      worldSprites[i].setPosition(sf::Vector2f(x * wd.tileWidth * wd.xScale, y * wd.tileHeight * wd.yScale));
    }
  }
}

void World::Update(double deltaTime)
{
}

void World::Draw(sf::RenderWindow& window)
{
  for (int i = 0; i < wd.dataLength; i++)
  {
    window.draw(worldSprites[i]);
  }
}
