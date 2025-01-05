#include "TileSelector.h"


void TileSelector::Initialize(WorldGrid& worldGrid)
{
  selectorBody.setSize(sf::Vector2f(worldGrid.tileSize, worldGrid.tileSize));
  selectorBody.setFillColor(sf::Color::Transparent);
  selectorBody.setOutlineThickness(4.f);
  selectorBody.setOutlineColor(sf::Color::Yellow);
}

void TileSelector::Update(WorldGrid& worldGrid)
{
  this->selectorPosition = sf::Vector2f(worldGrid.mousePosGrid.x * worldGrid.tileSize, worldGrid.mousePosGrid.y * worldGrid.tileSize);
  selectorBody.setPosition(this->selectorPosition);
}
