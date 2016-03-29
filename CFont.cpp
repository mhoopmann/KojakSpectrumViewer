#include "CFont.h"

CFont::CFont(){
  fontSize=16;
  font=NULL;
  rend = NULL;
  for(int j=0;j<2;j++){
    for(int i=0;i<128;i++){
      texture_8[j][i]=NULL;
      texture_10[j][i]=NULL;
      texture_12[j][i]=NULL;
      texture_16[j][i]=NULL;
    }
  }
  TTF_Init();
}

CFont::~CFont(){
  rend = NULL;
  for(int j=0;j<2;j++){
    for(int i=0;i<128;i++){
      if(texture_8[j][i]!=NULL) {
        SDL_DestroyTexture(texture_8[j][i]);
        texture_8[j][i]=NULL;
      }
      if(texture_10[j][i]!=NULL) {
        SDL_DestroyTexture(texture_10[j][i]);
        texture_10[j][i]=NULL;
      }
      if(texture_12[j][i]!=NULL) {
        SDL_DestroyTexture(texture_12[j][i]);
        texture_12[j][i]=NULL;
      }
      if(texture_16[j][i]!=NULL) {
        SDL_DestroyTexture(texture_16[j][i]);
        texture_16[j][i]=NULL;
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
  switch(fontSize){
  case 8:
    return rect_8[106].h;
  case 10:
    return rect_10[106].h;
  case 12:
    return rect_12[106].h;
  case 16:
  default:
    return rect_16[106].h;
  }
}

int CFont::getStringWidth(char* str){
  int i=0;
  int index;
  for(size_t j=0;j<strlen(str);j++){
    index=(int)str[j];
    switch(fontSize){
    case 8:
      i+=rect_8[index].w;
      break;
    case 10:
      i+=rect_10[index].w;
      break;
    case 12:
      i+=rect_12[index].w;
      break;
    case 16:
    default:
      i+=rect_16[index].w;
      break;
    }
  }
  return i;
}

int CFont::getStringWidth(string str){
  return getStringWidth(&str[0]);
}

bool CFont::loadFont(char* fname){
  int i,j;
  for(j=0;j<2;j++){
    font = TTF_OpenFont(fname,8);
    if(font==NULL) return false;
    for(i=32;i<127;i++){
      if(!setText((char)i,texture_8[j][i],j)) return false;
      SDL_QueryTexture(texture_8[j][i],NULL,NULL,&rect_8[i].w,&rect_8[i].h);
    }
    TTF_CloseFont(font);
    font = TTF_OpenFont(fname,10);
    if(font==NULL) return false;
    for(i=32;i<127;i++){
      if(!setText((char)i,texture_10[j][i],j)) return false;
      SDL_QueryTexture(texture_10[j][i],NULL,NULL,&rect_10[i].w,&rect_10[i].h);
    }
    TTF_CloseFont(font);
    font = TTF_OpenFont(fname,12);
    if(font==NULL) return false;
    for(i=32;i<127;i++){
      if(!setText((char)i,texture_12[j][i],j)) return false;
      SDL_QueryTexture(texture_12[j][i],NULL,NULL,&rect_12[i].w,&rect_12[i].h);
    }
    TTF_CloseFont(font);
    font = TTF_OpenFont(fname,16);
    if(font==NULL) return false;
    for(i=32;i<127;i++){
      if(!setText((char)i,texture_16[j][i],j)) return false;
      SDL_QueryTexture(texture_16[j][i],NULL,NULL,&rect_16[i].w,&rect_16[i].h);
    }
    TTF_CloseFont(font);
    font=NULL;
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

  switch(fontSize){
    case 8:
      for(i=0;i<strlen(str);i++){
        index=(int)str[i];
        r=rect_8[index];
        r.x=posX;
        r.y=posY;
        if(rotate){
          p.x=0;
          p.y=0;
          SDL_RenderCopyEx(rend,texture_8[color][index],NULL,&r,-90.0,&p,SDL_FLIP_NONE);
          posY-=r.w;
        } else {
          SDL_RenderCopy(rend,texture_8[color][index],NULL,&r);
          posX+=r.w;
        }
      }
      break;
    case 10:
      for(i=0;i<strlen(str);i++){
        index=(int)str[i];
        r=rect_10[index];
        r.x=posX;
        r.y=posY;
        if(rotate){
          p.x=0;
          p.y=0;
          SDL_RenderCopyEx(rend,texture_10[color][index],NULL,&r,-90.0,&p,SDL_FLIP_NONE);
          posY-=r.w;
        } else {
          SDL_RenderCopy(rend,texture_10[color][index],NULL,&r);
          posX+=r.w;
        }
      }
      break;
    case 12:
      for(i=0;i<strlen(str);i++){
        index=(int)str[i];
        r=rect_12[index];
        r.x=posX;
        r.y=posY;
        if(rotate){
          p.x=0;
          p.y=0;
          SDL_RenderCopyEx(rend,texture_12[color][index],NULL,&r,-90.0,&p,SDL_FLIP_NONE);
          posY-=r.w;
        } else {
          SDL_RenderCopy(rend,texture_12[color][index],NULL,&r);
          posX+=r.w;
        }
      }
      break;
    case 16:
    default:
      for(i=0;i<strlen(str);i++){
        index=(int)str[i];
        r=rect_16[index];
        r.x=posX;
        r.y=posY;
        if(rotate){
          p.x=0;
          p.y=0;
          SDL_RenderCopyEx(rend,texture_16[color][index],NULL,&r,-90.0,&p,SDL_FLIP_NONE);
          posY-=r.w;
        } else {
          SDL_RenderCopy(rend,texture_16[color][index],NULL,&r);
          posX+=r.w;
        }
      }
      break;
  }
}

void CFont::render(int x, int y, string s, int color, bool rotate) {
  render(x,y,&s[0],color,rotate);
}

void CFont::setFontSize(int sz){
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
