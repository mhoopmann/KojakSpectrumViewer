/*
Copyright 2016, Michael R. Hoopmann, Institute for Systems Biology

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#ifndef _CDISPLAY_H
#define _CDISPLAY_H

#ifdef GCC
#include <SDL2/SDL.h>
#else
#include <SDL.h>
#endif

//#include <stdio.h>
#include <string>
#include <vector>
#include "Structs.h"

using namespace std;

class CDisplay {
public:
  CDisplay();
  ~CDisplay();

  //SDL_Surface*  screenSurface;
  SDL_Cursor*   cursors[4];
  SDL_Renderer* renderer;
  SDL_Window*   window;
  
  kvPalette pal;
  vector<kvColor> txtColors;
  
  bool  getWindowSize(int& w, int& h);
  bool  init();

private:
  int screenHeight;
  int screenWidth;

  void    defaultColors();
  kvColor extractColors(const char* s);
  kvColor extractColors(string& s);
  int     extractColorsText(const char* s);
  int     extractColorsText(string& s);
  void    readPalette();

};

#endif
