#include "WorldGrid.h"


void WorldGrid::Initialize()
{
  tileMap.resize(mapWidth, std::vector<Tile>());

  for (int x = 0; x < mapWidth; x++)
  {
    tileMap[x].resize(mapWidth, Tile());
    for (int y = 0; y < mapHeight; y++)
    {
      tileMap[x][y].shape.setSize(sf::Vector2f(tileSize, tileSize));
      tileMap[x][y].shape.setOutlineThickness(1.f);
      tileMap[x][y].shape.setOutlineColor(sf::Color(150, 150, 150));
      tileMap[x][y].shape.setPosition(x * tileSize, y * tileSize);
      
      if (y < 25)
      {
        tileMap[x][y].shape.setFillColor(sf::Color::Cyan);
        tileMap[x][y].hasCollision = false;
        tileMap[x][y].type = 0;
      }

      else if (y >= 25 && tileMap[x][y - 1].type == 0)
      {
        tileMap[x][y].shape.setFillColor(sf::Color::Green);
        tileMap[x][y].hasCollision = true;
        tileMap[x][y].type = 1;
      }
      else
      {
        tileMap[x][y].shape.setFillColor(sf::Color(139, 69, 19));
        tileMap[x][y].hasCollision = true;
        tileMap[x][y].type = 2;
      }
    }
  }
}

void WorldGrid::ChangeTile(int selectedType)
{
  Tile& hoveredTile = tileMap[mousePosGrid.x][mousePosGrid.y];

  if (selectedType == 0)
  {
    hoveredTile.hasCollision = false;
    hoveredTile.shape.setFillColor(sf::Color::Cyan);
  }
  else if (selectedType == 1)
  {
    hoveredTile.hasCollision = true;
    hoveredTile.shape.setFillColor(sf::Color::Green);
  }
  else if(selectedType == 2)
  {
    hoveredTile.hasCollision = true;
    hoveredTile.shape.setFillColor(sf::Color(139, 69, 19));
  }

  hoveredTile.type = selectedType;
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

void WorldGrid::Render(sf::RenderWindow& window, sf::View& view)
{
  fromX = view.getCenter().x / tileSize - 27; // 27
  toX = view.getCenter().x / tileSize + 28; // 28
  fromY = view.getCenter().y / tileSize - 15; // 15
  toY = view.getCenter().y / tileSize + 16; // 16

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
      window.draw(tileMap[x][y].shape);
    }
  }
}
