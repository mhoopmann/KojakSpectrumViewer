#include "CFont.h"

CFont::CFont(){
  fontSize=16;
  font=NULL;
  rend = NULL;

  int i, j, k;

  for(i=0; i<21; i++){
    for(j=0; j<2; j++){
      for(k=0; k<128; k++){
        texture[i][j][k]=NULL;
      }
    }
  }

  TTF_Init();
}

CFont::~CFont(){
  rend = NULL;

  int i, j, k;

  for(i=0; i<21; i++){
    for(j=0; j<2; j++){
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
  for(j=0;j<2;j++){
    for(i=6; i<21; i++){
      font = TTF_OpenFont(fname, i);
      if(font==NULL) return false;
      for(k=32; k<127; k++){
        if(!setText((char)k, texture[i][j][k], j)) return false;
        SDL_QueryTexture(texture[i][j][k], NULL, NULL, &rect[i][k].w, &rect[i][k].h);
      }
      TTF_CloseFont(font);
      font=NULL;
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
      SDL_RenderCopyEx(rend, texture[fontSize][color][index], NULL, &r, -90.0, &p, SDL_FLIP_NONE);
      posY-=r.w;
    } else {
      SDL_RenderCopy(rend, texture[fontSize][color][index], NULL, &r);
      posX+=r.w;
    }
  }

}

void CFont::render(int x, int y, string s, int color, bool rotate) {
  render(x,y,&s[0],color,rotate);
}

void CFont::setFontSize(int sz){
  if(sz<6) sz=6;
  if(sz>20) sz=20;
  fontSize=sz;
}

void CFont::setRenderer(SDL_Renderer* renderer){
  rend = renderer;
}

bool CFont::setText(char c, SDL_Texture*& dest, int color){
  SDL_Color col = {255,255,255};
  char str[2];
  str[0]=c;
  str[1]='\0';
  if(dest!=NULL) SDL_DestroyTexture(dest);
  if(color==1) {
    col.r=44;
    col.g=62;
    col.b=80;
  }
  SDL_Surface* surf = TTF_RenderText_Blended(font,str,col);
  if(surf==NULL) return false;
  dest = SDL_CreateTextureFromSurface(rend,surf);
  if(dest==NULL) return false;
  SDL_FreeSurface(surf);
  return true;
}
