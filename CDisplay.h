#ifndef _CDISPLAY_H
#define _CDISPLAY_H

#include <SDL.h>
//#include <stdio.h>
#include <string>
#include "Structs.h"

using namespace std;

class CDisplay {
public:
  CDisplay();
  ~CDisplay();

  //SDL_Surface*  screenSurface;
  SDL_Renderer* renderer;
  SDL_Window*   window;
  
  kvPalette pal;
  
  bool  getWindowSize(int& w, int& h);
  bool  init();

private:
  int           screenHeight;
  int           screenWidth;

  kvColor extractColors(string& s);
  void    readPalette();

};

#endif