#include "UI.h"


void UI::Initialize()
{
  font.loadFromFile("assets/fonts/SegUIVar.ttf");
  gridInfoText.setCharacterSize(24);
  gridInfoText.setFillColor(sf::Color(255, 10, 255));
  gridInfoText.setFont(font);
  gridInfoText.setPosition(550.f, 20.f);
  gridInfoText.setString(" NO TEXT ");

  uiText.setCharacterSize(36);
  uiText.setFillColor(sf::Color::Yellow);
  uiText.setFont(font);
  uiText.setPosition(20.f, 900.f);
  uiText.setString(" NO TEXT ");

  controlsText.setCharacterSize(36);
  controlsText.setFillColor(sf::Color::Red);
  controlsText.setFont(font);
  controlsText.setPosition(1500.f, 20.f);
  controlsText.setString(" NO TEXT ");

  playerInfoText.setCharacterSize(30);
  playerInfoText.setFillColor(sf::Color::White);
  playerInfoText.setFont(font);
  playerInfoText.setPosition(20.f, 300.f);
  playerInfoText.setString(" NO TEXT ");

  inventoryText.setCharacterSize(18);
  inventoryText.setFillColor(sf::Color::Black);
  inventoryText.setFont(font);
  inventoryText.setString(" NO TEXT ");
}

void UI::Update(WorldGrid& worldGrid, TileSelector& tileSelector, Player& player, Inventory& inventory)
{
  std::stringstream ss;
  Tile& hoveredTile = worldGrid.tileMap[worldGrid.mousePosGrid.x][worldGrid.mousePosGrid.y];
  std::string collisionStatus = hoveredTile.hasCollision ? "Yes" : "No";

  // Mouse positions and world info
  ss << "Window: " << worldGrid.mousePosWindow.x << ", " << worldGrid.mousePosWindow.y << std::endl
    << "View: " << worldGrid.mousePosView.x << ", " << worldGrid.mousePosView.y << std::endl
    << "Grid: " << worldGrid.mousePosGrid.x << ", " << worldGrid.mousePosGrid.y << std::endl 
    << "Block: " << worldGrid.blocks[hoveredTile.type] << std::endl
    << "Collision: " << collisionStatus << std::endl;
  gridInfoText.setString(ss.str());
  ss.str(std::string());

  // GUI placeholder
  ss << "Selected block: " << worldGrid.blocks[tileSelector.selectedType] << std::endl;
  uiText.setString(ss.str());
  ss.str(std::string());

  // Controls
  ss << "Move: A, D" << std::endl
    << "Jump: SPACE" << std::endl
    << "Select blocks: 1, 2, 3" << std::endl
    << "Debug info: TAB" << std::endl
    << "Close window: ESC";
  controlsText.setString(ss.str());
  ss.str(std::string());

  //Player info
  ss << "Grounded: " << player.grounded << std::endl
    << "Colliding: " << player.colliding << std::endl
    << "X: " << std::floor((player.body.getPosition().x + player.width) / worldGrid.tileSize) << std::endl
    << "Y " << std::floor((player.body.getPosition().y + player.height) / worldGrid.tileSize) << std::endl
    << "Velocity X: " << player.velocity.x << std::endl
    << "Velocity Y: " << player.velocity.y << std::endl
    << "Jumping: " << player.jumping << std::endl
    << "Input vel X: " << player.inputVelocity.x << std::endl
    << "Click Pos: " << tileSelector.clickPosition.x << ", " << tileSelector.clickPosition.y << std::endl
    << "Last mined: " << worldGrid.blocks[tileSelector.minedType] << std::endl
    << "In inventory: " << inventory.inInventory << std::endl
    << "Inventory pos: " << inventory.mousePosInventory.x << ", " << inventory.mousePosInventory.y << std::endl
    << "Inventory click pos: " << inventory.clickPosition.x << ", " << inventory.clickPosition.y << "; ID: " << inventory.container[inventory.clickPosition.x][inventory.clickPosition.y].id << std::endl
    << "Mining: " << player.mining << std::endl;

  playerInfoText.setString(ss.str());
  ss.str(std::string());

  for (int x = 0; x < inventory.xCellCount; x++)
  {
    for (int y = 0; y < inventory.yCellCount; y++)
    {
      inventory.container[x][y].text.setPosition(sf::Vector2f(x * (inventory.renderCellSize + inventory.gap) + inventory.margin, y * (inventory.renderCellSize + inventory.gap) + inventory.margin));
      //inventory.container[x][y].text = std::to_string(inventory.container[x][y].quantity);
      ss << inventory.container[x][y].quantity;
      inventoryText.setString(ss.str());
    }
  }
}

void UI::Render(sf::RenderWindow& window)
{
  if (visible)
  {
    window.draw(gridInfoText);
    window.draw(playerInfoText);
  }
  window.draw(uiText);
  window.draw(controlsText);
  
}
