#pragma once

#include <string>


// Files and Directories
std::wstring workingDir;
std::wstring mainThemeFile;

// Window's dimension and info.
bool FULLSCREEN = true;
int windowWidth = (FULLSCREEN ? 2560 : 1000);
int windowHeight = (FULLSCREEN ? 1440 : 800);
int windowCenter[2] = { windowWidth / 2, windowHeight / 2 };
int window_menu_id;
float windowMatrix[16];
bool sound_on = true;
