#include "WorldGrid.h"


void WorldGrid::Initialize()
{
  tileMap.resize(mapWidth, std::vector<Tile>());

  if (tileAtlasTexture.loadFromFile("assets/textures/texture_test.png"))
  {
    xTileCount = tileAtlasTexture.getSize().x / tileSize;
    yTileCount = tileAtlasTexture.getSize().y / tileSize;

    std::cout << "Successfully loaded tile atlas." << std::endl;

    tileCount = xTileCount * yTileCount;

    atlasTiles = new AtlasTile[tileCount];

    // Tile atlas loop
    for (int x = 0; x < xTileCount; x++)
    {
      for (int y = 0; y < yTileCount; y++)
      {
        int index = x + y * xTileCount;

        atlasTiles[index].id = index;
        atlasTiles[index].position = sf::Vector2f(x * tileSize, y * tileSize);
      }
    }
  }
  else
  {
    std::cout << "Failed to load tilesheet." << std::endl;
  }


  // World gen
  for (int x = 0; x < mapWidth; x++)
  {
    tileMap[x].resize(mapWidth, Tile());
    for (int y = 0; y < mapHeight; y++)
    {
      int i = x + y * mapWidth;


      tileMap[x][y].shape.setSize(sf::Vector2f(tileSize, tileSize));
      tileMap[x][y].shape.setPosition(x * tileSize, y * tileSize);
      tileMap[x][y].position = sf::Vector2f(x * tileSize, y * tileSize);
      
      // Tile generation conditions
      if (y < groundLevel)
      {
        PlaceTile(air, x, y);
      }
      //if the y values is within a specific range and the tile has air above it, grass is generated
      else if (y >= groundLevel && tileMap[x][y - 1].type == 0)
      {
        PlaceTile(grass, x, y);
      }
      else
      {
        PlaceTile(dirt, x, y);
      }

      tileMap[x][y].sprite.setTexture(tileAtlasTexture);
      tileMap[x][y].sprite.setTextureRect(sf::IntRect(
        atlasTiles[tileMap[x][y].type].position.x,
        atlasTiles[tileMap[x][y].type].position.y,
        tileSize,
        tileSize
      ));

      tileMap[x][y].sprite.setPosition(tileMap[x][y].position);
    }
  }
}


void WorldGrid::Update(sf::RenderWindow& window)
{
  mousePosWindow = sf::Mouse::getPosition(window);
  mousePosView = window.mapPixelToCoords(mousePosWindow);

  if (mousePosView.x >= 0.f)
  {
    mousePosGrid.x = mousePosView.x / tileSizeU;
  }
  if (mousePosView.y >= 0.f)
  {
    mousePosGrid.y = mousePosView.y / tileSizeU;
  }

  if (mousePosGrid.x > mapWidth - 1)
  {
    mousePosGrid.x = mapWidth - 1;
  }
  if (mousePosGrid.y > mapHeight - 1)
  {
    mousePosGrid.y = mapHeight - 1;
  }
}

void WorldGrid::PlaceTile(int type, int xPos, int yPos)
{
  switch (type)
  {
  case 0:
    tileMap[xPos][yPos].hasCollision = false;
    tileMap[xPos][yPos].sprite.setTextureRect(sf::IntRect(
      atlasTiles[type].position.x,
      atlasTiles[type].position.y,
      tileSize,
      tileSize
    ));
    break;

  case 1:
    tileMap[xPos][yPos].hasCollision = true;

    tileMap[xPos][yPos].sprite.setTextureRect(sf::IntRect(
      atlasTiles[type].position.x,
      atlasTiles[type].position.y,
      tileSize,
      tileSize
    ));
    break;
    
  case 2:
    tileMap[xPos][yPos].hasCollision = true;
    
    tileMap[xPos][yPos].sprite.setTextureRect(sf::IntRect(
      atlasTiles[type].position.x,
      atlasTiles[type].position.y,
      tileSize,
      tileSize
    ));
    break;
  }
  tileMap[xPos][yPos].type = type;
}

void WorldGrid::Render(sf::RenderWindow& window, sf::View& view)
{
  // Ensures that only visible tiles are rendered
  fromX = view.getCenter().x / tileSize - 60;
  toX = view.getCenter().x / tileSize + 61;
  fromY = view.getCenter().y / tileSize - 34;
  toY = view.getCenter().y / tileSize + 35;

#pragma region TileRenderConditions

  if (fromX < 0)
  {
    fromX = 0;
  }
  else if (fromX >= mapWidth)
  {
    fromX = mapWidth - 1;
  }

  if (toX < 0)
  {
    toX = 0;
  }
  else if (toX >= mapWidth)
  {
    toX = mapWidth;
  }

  if (fromY < 0)
  {
    fromY = 0;
  }
  else if (fromY >= mapHeight)
  {
    fromY = mapHeight - 1;
  }

  if (toY < 0)
  {
    toY = 0;
  }
  else if (toY >= mapHeight)
  {
    toY = mapHeight;
  }
#pragma endregion


  for (int x = fromX; x < toX; x++)
  {
    for (int y = fromY; y < toY; y++)
    {
      window.draw(tileMap[x][y].sprite);
    }
  }
}
