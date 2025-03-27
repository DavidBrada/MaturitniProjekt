#include "WorldGrid.h"

void WorldGrid::Load()
{

  // World gen
  GenerateTerrain();
  InitializeCave();

  for (int i = 0; i < 5; i++)
  {
    SmoothCave(tileMap);
  }

  GenerateTunnels();
  FillTiles(); // Replaces air tiles with background blocks in caves

  GenerateStone();
  GenerateIron();
  GenerateCoal();
  PlaceGrass();
  GenerateTrees();
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
  case air:
    worldMap[xPos][yPos].hasCollision = false;
    worldMap[xPos][yPos].mineable = false;
    break;

  case dirt:
    worldMap[xPos][yPos].hasCollision = true;
    worldMap[xPos][yPos].mineable = true;
    break;
    
  case grass:
    worldMap[xPos][yPos].hasCollision = true;
    worldMap[xPos][yPos].mineable = true;
    break;

  case dirtBackground:
    worldMap[xPos][yPos].hasCollision = false;
    worldMap[xPos][yPos].mineable = false;
    break;

  case stone:
    worldMap[xPos][yPos].hasCollision = true;
    worldMap[xPos][yPos].mineable = true;
    break;

  case iron:
    worldMap[xPos][yPos].hasCollision = true;
    worldMap[xPos][yPos].mineable = true;
    break;

  case treeTrunk:
    worldMap[xPos][yPos].hasCollision = false;
    worldMap[xPos][yPos].mineable = true;
    break;
  case leaves:
    worldMap[xPos][yPos].hasCollision = false;
    worldMap[xPos][yPos].mineable = true;
    break;
  case workbench:
    worldMap[xPos][yPos].hasCollision = false;
    worldMap[xPos][yPos].mineable = true;
    break;
  case coal:
    worldMap[xPos][yPos].hasCollision = true;
    worldMap[xPos][yPos].mineable = true;
    break;
  case treeBottom:
    worldMap[xPos][yPos].hasCollision = false;
    worldMap[xPos][yPos].mineable = true;
  case branchTrunk:
    worldMap[xPos][yPos].hasCollision = false;
    worldMap[xPos][yPos].mineable = true;
    break;
    // This is like the OR logic operator
  case branch1:
  case branch2:
    worldMap[xPos][yPos].hasCollision = false;
    worldMap[xPos][yPos].mineable = true;

  }

  SetSprite(type, xPos, yPos, worldMap);
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

void WorldGrid::SetSprite(int type, float xPos, float yPos, std::vector<std::vector<Tile>>& worldMap)
{
  worldMap[xPos][yPos].sprite.setTextureRect(sf::IntRect(
    atlasTiles[type].position.x,
    atlasTiles[type].position.y,
    tileSize,
    tileSize
  ));
}

void WorldGrid::GenerateTerrain()
{
  FastNoiseLite terrainNoise;
  terrainNoise.SetSeed(worldSeed);
  terrainNoise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);

  for (int x = 0; x < mapWidth; x++)
  {
    tileMap[x].resize(mapWidth, Tile());

    float noiseValue = terrainNoise.GetNoise((float)x, 0.0f); // Multiplying x value will make the terrain smoother (making hills steeper or making the incline more gradual)
    int heightOffset = (int)(noiseValue * 90.0f);
    terrainHeight = groundLevel + heightOffset;

    if (terrainHeight > groundLevel)
    {
      terrainHeight = groundLevel + 2;
    }

    terrainHeightValues[x] = terrainHeight;

    for (int y = 0; y < mapHeight; y++)
    {
      tileMap[x][y].shape.setSize(sf::Vector2f(tileSize, tileSize));
      tileMap[x][y].shape.setPosition(x * tileSize, y * tileSize);
      tileMap[x][y].position = sf::Vector2f(x * tileSize, y * tileSize);

      if (x == mapWidth / 2 && y == terrainHeightValues[x])
      {
        playerSpawnPos = sf::Vector2i(x * tileSize, (y - 5) * tileSize);
      }
      
      if (y < terrainHeightValues[x])
      {
        PlaceTile(air, x, y, tileMap);
      }
      //if the y values is within a specific range and the tile has air above it, grass is generated
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

void WorldGrid::MineTile(Player* player, Settings& settings, TileSelector* tileSelector, Inventory* inventory, CraftingMenu* craftingMenu)
{
  player->mining = true;

  if (settings.instaBreak)
  {
    tileSelector->timeToMine = 0.f;

  }
  else
  {
    tileSelector->timeToMine = 0.5f;
  }

  if (tileSelector->clickPosition.x == mousePosGrid.x &&
      tileSelector->clickPosition.y == mousePosGrid.y &&
      tileMap[tileSelector->selectorPosition.x / tileSize][tileSelector->selectorPosition.y / tileSize].mineable){

    if (tileSelector->mineClock.getElapsedTime().asSeconds() >= tileSelector->timeToMine && !inventory->inInventory && !craftingMenu->inCrafting)
    {
      tileSelector->minedType = tileMap[tileSelector->selectorPosition.x / tileSize][tileSelector->selectorPosition.y / tileSize].type;

      if (tileSelector->minedType == treeTrunk || tileSelector->minedType == treeBottom || tileSelector->minedType == branchTrunk)
      {
        bool isTreeTop = false;
        for (int i = 0; i < 11; i++)
        {
          if (!isTreeTop)
          {
            if (tileMap[mousePosGrid.x][mousePosGrid.y - i].type == treeTrunk ||
                tileMap[mousePosGrid.x][mousePosGrid.y - i].type == treeBottom)
            {
              PlaceTile(air, mousePosGrid.x, mousePosGrid.y - i, tileMap);
            }
            else if (tileMap[mousePosGrid.x][mousePosGrid.y - i].type == branchTrunk)
            {
              for (int j = -1; j < 2; j++)
              {
                if (tileMap[mousePosGrid.x + j][mousePosGrid.y - i].type == branch1 ||
                    tileMap[mousePosGrid.x + j][mousePosGrid.y - i].type == branch2 ||
                    tileMap[mousePosGrid.x + j][mousePosGrid.y - i].type == branchTrunk)
                {
                  PlaceTile(air, mousePosGrid.x + j, mousePosGrid.y - i, tileMap);
                }
              }
            }
            inventory->StoreItem(treeTrunk, *this); // CHANGE to a generic wood item

            if (tileMap[mousePosGrid.x][mousePosGrid.y - i].type == leaves)
            {
              isTreeTop = true;
              for (int j = -2; j < 3; j++)
              {
                if (tileMap[mousePosGrid.x + j][mousePosGrid.y - i].type == leaves)
                {
                  PlaceTile(air, mousePosGrid.x + j, mousePosGrid.y - i, tileMap);
                }
              }
            }
          }
          else
          {
            for (int j = -2; j < 3; j++)
            {
              if (tileMap[mousePosGrid.x + j][mousePosGrid.y - i].type == leaves)
              {
                PlaceTile(air, mousePosGrid.x + j, mousePosGrid.y - i, tileMap);
              }
            }
          }
        }
      }
      else
      {
        inventory->StoreItem(tileSelector->minedType, *this);
      }
      
      if (mousePosGrid.y > terrainHeightValues[mousePosGrid.x])
      {
        PlaceTile(dirtBackground, mousePosGrid.x, mousePosGrid.y, tileMap);
      }
      else
      {
        PlaceTile(air, mousePosGrid.x, mousePosGrid.y, tileMap);
      }
    }
  }
  else
  {
    tileSelector->mineClock.restart();
    tileSelector->clickPosition.x = mousePosGrid.x;
    tileSelector->clickPosition.y = mousePosGrid.y;
    player->mining = false;
  }
}

// Fills the tiles randomly with air or dirt. This is then processed by the SmoothCave() function to generate caves
void WorldGrid::InitializeCave()
{
  for (int x = 0; x < mapWidth; x++)
  {
    
    for (int y = terrainHeightValues[x] + 10; y < mapHeight - 10; y++)
    {
      (rand() % 100 < 45) ? PlaceTile(1, x, y, tileMap) : PlaceTile(0, x, y, tileMap); // 45% solid, 55% air
    }
  }
}

void WorldGrid::GenerateTunnels()
{
  FastNoiseLite caveNoise;

  seedOffset = 956275;

  caveNoise.SetSeed(worldSeed + seedOffset);
  caveNoise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
  caveNoise.SetFrequency(0.03f);

  for (int x = 0; x < mapWidth; x++)
  {
    for (int y = terrainHeightValues[x]; y < mapHeight - 10; y++)
    {
      float noiseValue = caveNoise.GetNoise((float)x, (float)y);
      if (fabs(noiseValue) < 0.1f && y > 180)
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
    for (int y = terrainHeightValues[x] + 10; y < mapHeight - 11; y++)
    {
      int neighborCount = 0;

      //This checks for surrounding tiles of every tile
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

      // if a tile is touching more than 4 solid tiles, it gets replaced with air
      // For whatever reason this actually works
      if (neighborCount > 4) PlaceTile(0, x, y, newMap);
      else PlaceTile(1, x, y, newMap);
    }
  }
  tileMap = newMap;
}

void WorldGrid::GenerateStone()
{
  FastNoiseLite stoneNoise;

  seedOffset = 68365;
  stoneNoise.SetSeed(worldSeed + seedOffset);
  stoneNoise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
  stoneNoise.SetFrequency(0.08f); // Controls patch size

  for (int x = 0; x < mapWidth; x++)
  {
    for (int y = terrainHeightValues[x] + 10; y < mapHeight; y++)
    {
      float noiseValue = stoneNoise.GetNoise((float)x, (float)y);

      if (tileMap[x][y].type == dirt && noiseValue > 0.6f) // lower threshold value = more stone
      {
        PlaceTile(stone, x, y, tileMap);
      }
    }
  }
}

void WorldGrid::GenerateIron()
{
  FastNoiseLite ironNoise;

  seedOffset = 26631;
  ironNoise.SetSeed(worldSeed + seedOffset);
  ironNoise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
  ironNoise.SetFrequency(0.04f);

  for (int x = 0; x < mapWidth; x++)
  {
    for (int y = terrainHeightValues[x] + 20; y < mapHeight; y++) // Starts 20 tiles underground
    {
      float noiseValue = ironNoise.GetNoise((float)x, (float)y);

      if (tileMap[x][y].type == stone && noiseValue > 0.8f)
      {
        PlaceTile(iron, x, y, tileMap);
      }
    }
  }
}

void WorldGrid::PlaceGrass()
{
  for (int x = 0; x < mapWidth; x++)
  {
    for (int y = 0; y < mapHeight; y++)
    {
      if (tileMap[x][terrainHeightValues[x]].type == dirt &&
          tileMap[x][terrainHeightValues[x] - 1].type == air &&
          y >= terrainHeightValues[x])
      {
        PlaceTile(grass, x, terrainHeightValues[x] - 1, tileMap);
        terrainHeightValues[x] -= 1;
      }
    }
  }
}

void WorldGrid::GenerateTrees()
{
  for (int x = 5; x < mapWidth - 10; x += (rand() % 10 + 5))
  {
    int yGround = terrainHeightValues[x];

    if (tileMap[x][yGround - 1].type == air && tileMap[x][yGround].type == grass)
    {
      PlaceTree(x, yGround - 1);
    }
  }
}

void WorldGrid::PlaceTree(int x, int yGround)
{
  int treeHeight = rand() % 6 + 6; // Height of a tree excluding leaves min: 6, max: 11

  for (int y = yGround; y > yGround - treeHeight; y--)
  {
    if (y == yGround)
    {
      PlaceTile(treeBottom, x, y, tileMap);
    }
    else
    {
      PlaceTile(treeTrunk, x, y, tileMap);
    }
  }

  // Leaf generation
  int leafStart = yGround - treeHeight;
  for (int dx = -2; dx <= 2; dx++)
  {
    for (int dy = -2; dy <= 1; dy++)
    {
      if (rand() % 100 < 75)
      {
        PlaceTile(leaves, x + dx, leafStart + dy, tileMap);
      }
    }
  }

  for (int y = yGround - 1; y > yGround - treeHeight + 2; y--)
  {
    if (rand() % 100 < 80)
    {
      int branchType;
      branchType = rand() % 2;

      if (rand() % 2 < 1){
        // Flip the branch and trunk texture horizontally
        tileMap[x - 1][y].sprite.setOrigin(tileSize, 0);
        tileMap[x - 1][y].sprite.setScale(-1.f, 1.f);

        if (branchType == 1)
        {
          PlaceTile(branch1, x - 1, y, tileMap);
        }
        else
        {
          PlaceTile(branch2, x - 1, y, tileMap);
        }

        tileMap[x][y].sprite.setOrigin(tileSize, 0);
        tileMap[x][y].sprite.setScale(-1.f, 1.f);
      }
      else
      {
        if (branchType == 1)
        {
          PlaceTile(branch1, x + 1, y, tileMap);
        }
        else
        {
          PlaceTile(branch2, x + 1, y, tileMap);
        }
      }

      PlaceTile(branchTrunk, x, y, tileMap);
    }
  }
}

void WorldGrid::GenerateCoal()
{
  FastNoiseLite coalNoise;

  seedOffset = 69420;
  coalNoise.SetSeed(worldSeed + seedOffset);
  coalNoise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
  coalNoise.SetFrequency(0.05f);

  for (int x = 0; x < mapWidth; x++)
  {
    for (int y = terrainHeightValues[x] + 10; y < mapHeight; y++) // Starts 10 tiles underground
    {
      float noiseValue = coalNoise.GetNoise((float)x, (float)y);

      if ((tileMap[x][y].type == stone || tileMap[x][y].type == dirt) && noiseValue > 0.8f)
      {
        PlaceTile(coal, x, y, tileMap);
      }
    }
  }
}

WorldGrid::~WorldGrid()
{
}

void WorldGrid::FillTiles()
{
  for (int x = 1; x < mapWidth; x++)
  {
    for (int y = terrainHeightValues[x]; y < mapHeight - 1; y++)
    {
      if (y > terrainHeightValues[x] && tileMap[x][y].type == 0)
      {
        PlaceTile(dirtBackground, x, y, tileMap);
      }
    }
  }
}