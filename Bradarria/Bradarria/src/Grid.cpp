#include "Grid.h"

Grid::Grid()
{
}

Grid::~Grid()
{
}

void Grid::Initialize()
{
  line.setSize(sf::Vector2f(100, 2));
  line.setPosition(sf::Vector2f(100, 200));
}

void Grid::Load()
{
}

void Grid::Update(double deltaTime)
{
}

void Grid::Draw(sf::RenderWindow& window)
{
  window.draw(line);
}
