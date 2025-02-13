#pragma once
struct Settings
{
  bool instaBreak; // Toggles if blocks should be mined instantly or not
  int fpsLimit;
  int windowWidth = 1920;
  int windowHeight = 1080;

  void Load();
};

