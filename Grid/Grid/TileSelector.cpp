#include "TileSelector.h"


void TileSelector::Initialize(WorldGrid& worldGrid)
{
  selectorBody.setOutlineThickness(4.f);
  selectorBody.setSize(sf::Vector2f(worldGrid.tileSize - selectorBody.getOutlineThickness() / 2, worldGrid.tileSize - selectorBody.getOutlineThickness() / 2));
  selectorBody.setFillColor(sf::Color::Transparent);
  selectorBody.setOutlineColor(sf::Color::Yellow);
}

void TileSelector::Update(WorldGrid& worldGrid)
{
  selectorPosition = sf::Vector2f(worldGrid.mousePosGrid.x * worldGrid.tileSize, worldGrid.mousePosGrid.y * worldGrid.tileSize);
  selectorBody.setPosition(selectorPosition);
}
