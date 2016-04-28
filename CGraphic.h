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

#ifndef _CGRAPHIC_H
#define _CGRAPHIC_H

#ifdef GCC
#include <SDL2/SDL.h>
#else
#include <SDL.h>
#endif

#include <stdio.h>
#include <string>

using namespace std;

class CGraphic {
public:
  CGraphic();
  ~CGraphic();
  
  SDL_Texture* texture;
  SDL_Surface* surface;

  bool      createImages(int szX, int szY, int canX, int canY);
  bool      createImages(char* fn);
  SDL_Rect  getRect(int index);
  bool      loadTexture(char* fn, SDL_Renderer* rend, bool surf=false, bool alpha=false, Uint8 r=0, Uint8 g=0, Uint8 b=0);
  void      render(SDL_Renderer* rend, int index, int posX, int posY, int w=0, int h=0);
  int       size();

private:

  int       imageCount;
  SDL_Rect* images;
  

};

#endif
