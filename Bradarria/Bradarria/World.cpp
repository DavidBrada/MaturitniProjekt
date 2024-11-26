#include "World.h"

World::World() : 
  tileWidth(18), tileHeight(18), totalTilesX(0), totalTilesY(0)
{
}

World::~World()
{
}

void World::Initialize()
{
  if (tileSheetTexture.loadFromFile("Assets/World/tilemap_packed.png"))
  {
    totalTilesX = tileSheetTexture.getSize().x / tileWidth;
    totalTilesY = tileSheetTexture.getSize().y / tileHeight;

    std::cout << totalTilesX << std::endl;
    std::cout << totalTilesY << std::endl;

    std::cout << "Tilesheet loaded." << std::endl;
    
    totalTiles = totalTilesX * totalTilesY;

    tiles = new WorldTile[totalTiles]; // 'new' keyword creates the array on heap (RAM) instead of stack (used for variables; has size of ~1MB). It's better when the array size is unknown


    for (int y = 0; y < totalTilesY; y++)
    {
      for (int x = 0; x < totalTilesX; x++)
      {
        int i = x + y * totalTilesX;

        tiles[i].id = i;
        tiles[i].position = sf::Vector2i(x * tileWidth, y * tileHeight);
      }
    }

  }
  else
  {
    std::cout << "Failed to load tilesheet." << std::endl;
  }


  for (int y = 0; y < 2; y++)
  {
    for (int x = 0; x < 3; x++)
    {
      int i = x + y * 3;

      int index = mapNumbers[i];

      mapSprites[i].setTexture(tileSheetTexture);
      mapSprites[i].setTextureRect(sf::IntRect(
        tiles[index].position.x,
        tiles[index].position.y,
        tileWidth,
        tileHeight
      ));
      mapSprites[i].setScale(sf::Vector2f(5, 5));
      mapSprites[i].setPosition(sf::Vector2f(x * 18 * 5, y * 18 * 5));
    }
  }
}

void World::Update(double deltaTime)
{
}

void World::Draw(sf::RenderWindow& window)
{
  for (int i = 0; i < 6; i++)
  {
    window.draw(mapSprites[i]);
  }
}
