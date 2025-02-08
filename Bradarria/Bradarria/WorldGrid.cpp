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

  GenerateTerrain();
  InitializeCave();

  for (int i = 0; i < 5; i++)
  { // Run smoothing multiple times
    SmoothCave(tileMap);
  }

  GenerateTunnels();
  FillTiles();
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

void WorldGrid::PlaceTile(int type, int xPos, int yPos, std::vector<std::vector<Tile>>& worldMap)
{
  switch (type)
  {
  case 0:
    worldMap[xPos][yPos].hasCollision = false;
    worldMap[xPos][yPos].sprite.setTextureRect(sf::IntRect(
      atlasTiles[type].position.x,
      atlasTiles[type].position.y,
      tileSize,
      tileSize
    ));
    break;

  case 1:
    worldMap[xPos][yPos].hasCollision = true;

    worldMap[xPos][yPos].sprite.setTextureRect(sf::IntRect(
      atlasTiles[type].position.x,
      atlasTiles[type].position.y,
      tileSize,
      tileSize
    ));
    break;
    
  case 2:
    worldMap[xPos][yPos].hasCollision = true;
    
    worldMap[xPos][yPos].sprite.setTextureRect(sf::IntRect(
      atlasTiles[type].position.x,
      atlasTiles[type].position.y,
      tileSize,
      tileSize
    ));
    break;

  case 3:
    worldMap[xPos][yPos].hasCollision = false;

    worldMap[xPos][yPos].sprite.setTextureRect(sf::IntRect(
      atlasTiles[type].position.x,
      atlasTiles[type].position.y,
      tileSize,
      tileSize
    ));
    break;
  }
  worldMap[xPos][yPos].type = type;
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

void WorldGrid::GenerateTerrain()
{
  FastNoiseLite terrainNoise;

  terrainNoise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);

  for (int x = 0; x < mapWidth; x++)
  {
    tileMap[x].resize(mapWidth, Tile());

    float noiseValue = terrainNoise.GetNoise((float)x, 0.0f); // Scale for smoothness
    int heightOffset = (int)(noiseValue * 30.0f); // Adjust amplitude
    terrainHeight = groundLevel + heightOffset; // Shift ground level
    
    if (terrainHeight > groundLevel + 2.f)
    {
      terrainHeight = groundLevel + 2.f;
    }

    if (x == mapWidth / 2)
    {
      playerSpawnPos.x = x * tileSize;
    }

    for (int y = 0; y < mapHeight; y++)
    {
      tileMap[x][y].shape.setSize(sf::Vector2f(tileSize, tileSize));
      tileMap[x][y].shape.setPosition(x * tileSize, y * tileSize);
      tileMap[x][y].position = sf::Vector2f(x * tileSize, y * tileSize);

      if (y == terrainHeight && x == mapWidth / 2)
      {
        playerSpawnPos.y = (y - 5) * tileSize;
      }
      
      if (y < terrainHeight)
      {
        PlaceTile(air, x, y, tileMap);
      }
      //if the y values is within a specific range and the tile has air above it, grass is generated
      else if (y >= terrainHeight && tileMap[x][y - 1].type == 0)
      {
        PlaceTile(grass, x, y, tileMap);
      }
      else
      {
        PlaceTile(dirt, x, y, tileMap);
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

void WorldGrid::InitializeCave()
{
  for (int x = 0; x < mapWidth; x++)
  {
    for (int y = terrainHeight + 10; y < mapHeight - 10; y++)
    {
      (rand() % 100 < 45) ? PlaceTile(1, x, y, tileMap) : PlaceTile(0, x, y, tileMap); // 45% solid, 55% air
    }
  }
}

void WorldGrid::GenerateTunnels()
{
  FastNoiseLite caveNoise;

  caveNoise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
  caveNoise.SetFrequency(0.03f);

  for (int x = 0; x < mapWidth; x++)
  {
    for (int y = 0; y < mapHeight - 10; y++)
    {
      float noiseValue = caveNoise.GetNoise((float)x, (float)y);
      if (fabs(noiseValue) < 0.1f)
      {
        PlaceTile(air, x, y, tileMap);
      }
    }
  }
}

void WorldGrid::SmoothCave(std::vector<std::vector<Tile>>& worldMap)
{
  std::vector<std::vector<Tile>> newMap = worldMap;

  for (int x = 1; x < mapWidth - 1; x++)
  {
    for (int y = terrainHeight + 10; y < mapHeight - 11; y++)
    {
      int neighborCount = 0;
      for (int dx = -1; dx <= 1; dx++)
      {
        for (int dy = -1; dy <= 1; dy++)
        {
          if (tileMap[x + dx][y + dy].type == 1)
          {
            neighborCount++;
          }
        }
      }

      // Rule: If more than 4 neighbors are solid, keep it solid.
      if (neighborCount > 4) PlaceTile(0, x, y, newMap);
      else PlaceTile(1, x, y, newMap);
    }
  }
  tileMap = newMap; // Replace old map with smoothed version
}

void WorldGrid::FillTiles()
{
  for (int x = 1; x < mapWidth - 1; x++)
  {
    for (int y = terrainHeight + 5; y < mapHeight; y++)
    {
      if (y > terrainHeight + 5 && tileMap[x][y].type == 0)
      {
        PlaceTile(dirtBackground, x, y, tileMap);
      }
    }
  }
}
