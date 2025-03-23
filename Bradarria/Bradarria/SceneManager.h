#pragma once
struct SceneManager
{
  int currentScene;

  enum scenes
  {
    mainMenu,
    worldSettingsMenu,
    game
  };

  SceneManager()
  {
    currentScene = mainMenu;
  }
};

