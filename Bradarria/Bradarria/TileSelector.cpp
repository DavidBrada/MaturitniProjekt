#include "TileSelector.h"


void TileSelector::Initialize(WorldGrid& worldGrid)
{
  selectorBody.setOutlineThickness(2.f);
  selectorBody.setSize(sf::Vector2f(worldGrid.tileSize - selectorBody.getOutlineThickness() / 2, worldGrid.tileSize - selectorBody.getOutlineThickness() / 2));
  selectorBody.setFillColor(sf::Color::Transparent);
  selectorBody.setOutlineColor(sf::Color::Yellow);
}

void TileSelector::Update(WorldGrid& worldGrid)
{
  selectorPosition = sf::Vector2f(worldGrid.mousePosGrid.x * worldGrid.tileSize, worldGrid.mousePosGrid.y * worldGrid.tileSize);
  selectorBody.setPosition(selectorPosition);

  canPlace = false;
  
  xCanPlaceFrom = worldGrid.mousePosGrid.x - 1;
  xCanPlaceTo = worldGrid.mousePosGrid.x + 2;
  yCanPlaceFrom = worldGrid.mousePosGrid.y - 1;
  yCanPlaceTo = worldGrid.mousePosGrid.y + 2;

#pragma region mapEdgeConditions

  if (xCanPlaceFrom < 0)
  {
    xCanPlaceFrom = 0;
  }
  else if (xCanPlaceFrom >= worldGrid.mapWidth)
  {
    xCanPlaceFrom = worldGrid.mapWidth - 1;
  }

  if (xCanPlaceTo < 0)
  {
    xCanPlaceTo = 0;
  }
  else if (xCanPlaceTo >= worldGrid.mapWidth)
  {
    xCanPlaceTo = worldGrid.mapWidth;
  }

  if (yCanPlaceFrom < 0)
  {
    yCanPlaceFrom = 0;
  }
  else if (yCanPlaceFrom >= worldGrid.mapHeight)
  {
    yCanPlaceFrom = worldGrid.mapHeight - 1;
  }

  if (yCanPlaceTo < 0)
  {
    yCanPlaceTo = 0;
  }
  else if (yCanPlaceTo >= worldGrid.mapHeight)
  {
    yCanPlaceTo = worldGrid.mapHeight;
  }
#pragma endregion


  for (int x = xCanPlaceFrom; x < xCanPlaceTo; x++)
  {
    for (int y = yCanPlaceFrom; y < yCanPlaceTo; y++)
    {
      if (worldGrid.tileMap[x][y].hasCollision)
      {
        canPlace = true;
      }
    }
  }
}
