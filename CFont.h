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

#ifndef _CFONT_H
#define _CFONT_H

#include <string>

#ifdef GCC
#include <SDL2/SDL_ttf.h>
#else
#include <SDL_ttf.h>
#endif

#include "CDisplay.h"

using namespace std;

class CFont {
public:
  CFont();
  ~CFont();

  int fontSize;
  
  int getFontHeight ();
  int getStringWidth(char* str);
  int getStringWidth(string str);
  bool loadFont     (char* fname);
  void setDisplay   (CDisplay* d);
  void setFontSize  (int sz);
  bool setText      (char c, SDL_Texture*& dest, int color=0);
  void render       (int x, int y, char* str, int color=0, bool rotate=false);
  void render       (int x, int y, string s, int color=0, bool rotate=false);

private:
  CDisplay*   display;
  TTF_Font*   font;

  SDL_Texture* texture[21][20][128]; //allow for up to 20 colors

  SDL_Rect rect[21][128];
  
  int height;
  int width;

};

#endif
