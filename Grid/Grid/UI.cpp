#include "UI.h"


void UI::Initialize()
{
  font.loadFromFile("assets/SegUIVar.ttf");
  gridInfoText.setCharacterSize(24);
  gridInfoText.setFillColor(sf::Color(255, 10, 255));
  gridInfoText.setFont(font);
  gridInfoText.setPosition(20.f, 20.f);
  gridInfoText.setString(" ");

  uiText.setCharacterSize(36);
  uiText.setFillColor(sf::Color::Yellow);
  uiText.setFont(font);
  uiText.setPosition(20.f, 900.f);
  uiText.setString(" NO TEXT ");

  controlsText.setCharacterSize(36);
  controlsText.setFillColor(sf::Color::Red);
  controlsText.setFont(font);
  controlsText.setPosition(1600.f, 20.f);
  controlsText.setString(" NO TEXT ");
}

void UI::Update(WorldGrid& worldGrid, TileSelector tileSelector)
{
  std::stringstream ss;
  Tile& hoveredTile = worldGrid.tileMap[worldGrid.mousePosGrid.x][worldGrid.mousePosGrid.y];
  std::string collisionStatus = hoveredTile.hasCollision ? "Yes" : "No";

  ss << "Window: " << worldGrid.mousePosWindow.x << ", " << worldGrid.mousePosWindow.y << std::endl
    << "View: " << worldGrid.mousePosView.x << ", " << worldGrid.mousePosView.y << std::endl
    << "Grid: " << worldGrid.mousePosGrid.x << ", " << worldGrid.mousePosGrid.y << std::endl 
    << "Block: " << worldGrid.blocks[hoveredTile.type] << std::endl
    << "Collision: " << collisionStatus << std::endl;

  gridInfoText.setString(ss.str());

  ss.str(std::string());

  ss << "Selected block: " << worldGrid.blocks[tileSelector.selectedType] << std::endl;

  uiText.setString(ss.str());

  ss.str(std::string());

  ss << "Select blocks: 1, 2, 3" << std::endl
    << "Close window: ESC";
  controlsText.setString(ss.str());
}

void UI::Render(sf::RenderWindow& window)
{
  if (visible)
  {
    window.draw(gridInfoText);
  }

  window.draw(uiText);
  window.draw(controlsText);
}
