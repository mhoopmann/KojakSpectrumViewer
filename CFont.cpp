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

#include "CFont.h"

CFont::CFont(){
  fontSize=16;
  font=NULL;
  display = NULL;

  int i, j, k;

  for(i=0; i<21; i++){
    for(j=0; j<20; j++){
      for(k=0; k<128; k++){
        texture[i][j][k]=NULL;
      }
    }
  }

  TTF_Init();
}

CFont::~CFont(){
  display = NULL;

  int i, j, k;

  for(i=0; i<21; i++){
    for(j=0; j<20; j++){
      for(k=0; k<128; k++){
        if(texture[i][j][k]!=NULL){
          SDL_DestroyTexture(texture[i][j][k]);
          texture[i][j][k]=NULL;
        }
      }
    }
  }

  if(font!=NULL){
    TTF_CloseFont(font);
    font=NULL;
  }
  TTF_Quit();
}

int CFont::getFontHeight(){
  return rect[fontSize][106].h;
}

int CFont::getStringWidth(char* str){
  int i=0;
  int index;
  for(size_t j=0;j<strlen(str);j++){
    index=(int)str[j];
    i+=rect[fontSize][index].w;
  }
  return i;
}

int CFont::getStringWidth(string str){
  return getStringWidth(&str[0]);
}

bool CFont::loadFont(char* fname){
  int i,j,k;
  for(j=0;j<display->txtColors.size();j++){
    for(i=6; i<21; i++){
      font = TTF_OpenFont(fname, i);
      if(font==NULL) return false;
      for(k=32; k<127; k++){
        if(!setText((char)k, texture[i][j][k], j)) return false;
        SDL_QueryTexture(texture[i][j][k], NULL, NULL, &rect[i][k].w, &rect[i][k].h);
      }
      TTF_CloseFont(font);
      font=NULL;
#ifdef GCC
      font = TTF_OpenFont("./Fonts/Carlito-alphabeta.ttf",i);
#else
      font = TTF_OpenFont("Fonts\\Carlito-alphabeta.ttf",i);
#endif
      setText(30, texture[i][j][30], j);
      SDL_QueryTexture(texture[i][j][30], NULL, NULL, &rect[i][30].w, &rect[i][30].h);
      setText(31, texture[i][j][31], j);
      SDL_QueryTexture(texture[i][j][31], NULL, NULL, &rect[i][31].w, &rect[i][31].h);
      TTF_CloseFont(font);
      font = NULL;
    }
  }
  return true;
}

void CFont::render(int x, int y, char* str, int color, bool rotate) {
  size_t i;
  int index;
  int posX=x;
  int posY=y;
  SDL_Rect r;
  SDL_Point p;

  for(i=0; i<strlen(str); i++){
    index=(int)str[i];
    r=rect[fontSize][index];
    r.x=posX;
    r.y=posY;
    if(rotate){
      p.x=0;
      p.y=0;
      SDL_RenderCopyEx(display->renderer, texture[fontSize][color][index], NULL, &r, -90.0, &p, SDL_FLIP_NONE);
      posY-=r.w;
    } else {
      SDL_RenderCopy(display->renderer, texture[fontSize][color][index], NULL, &r);
      posX+=r.w;
    }
  }

}

void CFont::render(int x, int y, string s, int color, bool rotate) {
  render(x,y,&s[0],color,rotate);
}

void CFont::setDisplay(CDisplay* d){
  display = d;
}

void CFont::setFontSize(int sz){
  if(sz<6) sz=6;
  if(sz>20) sz=20;
  fontSize=sz;
}

bool CFont::setText(char c, SDL_Texture*& dest, int color){
  SDL_Color col;
  char str[2];
  if (c == 30) str[0]='a';
  else if (c == 31) str[0] = 'b';
  else  str[0]=c;
  str[1]='\0';
  if(dest!=NULL) SDL_DestroyTexture(dest);
  col.r=display->txtColors[color].r;
  col.g=display->txtColors[color].g;
  col.b=display->txtColors[color].b;
  SDL_Surface* surf;
  surf = TTF_RenderText_Blended(font,str,col);
  if(surf==NULL) return false;
  dest = SDL_CreateTextureFromSurface(display->renderer,surf);
  if(dest==NULL) return false;
  SDL_FreeSurface(surf);
  return true;
}
