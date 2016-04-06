#include "CGraphic.h"
#include <iostream>

CGraphic::CGraphic(){
  texture = NULL;
  surface = NULL;
  images = NULL;
}

CGraphic::~CGraphic(){
  if(surface!=NULL) SDL_FreeSurface(surface);
  if(texture!=NULL) SDL_DestroyTexture(texture);
  surface=NULL;
  texture=NULL;
  if(images!=NULL) delete [] images;
  images = NULL;
}

SDL_Rect CGraphic::getRect(int index){
  return images[index];
}

bool CGraphic::loadTexture(char* fn, SDL_Renderer* rend, bool surf, bool alpha, Uint8 r, Uint8 g, Uint8 b) {

  if(texture!=NULL) SDL_DestroyTexture(texture);
  texture=NULL;

  if(surface!=NULL) SDL_FreeSurface(surface);
  surface=NULL;
  
  //Load image at specified path 
  surface = SDL_LoadBMP(fn);
  if( surface == NULL ) { 
    printf( "Unable to load image %s! SDL_image Error: %s\n", fn, SDL_GetError() );
    return false;
  } else {
    if(alpha) SDL_SetColorKey( surface, SDL_TRUE, SDL_MapRGB( surface->format, r, g, b ) );
    //Create texture from surface pixels 
    texture = SDL_CreateTextureFromSurface( rend, surface ); 
    if( texture == NULL ) { 
      printf( "Unable to create texture from %s! SDL Error: %s\n", fn, SDL_GetError() );
      return false;
    } 
    
    //Get rid of old loaded surface 
    if(!surf){
      SDL_FreeSurface( surface ); 
      surface=NULL;
    }
  } 
  
  return true; 
}

bool CGraphic::createImages(int szX, int szY, int canX, int canY){
  if(texture==NULL) return false;

  //get number of tiles
  int x=canX/szX; //texture->w/szX;
  int y=canY/szY; //texture->h/szY;
  imageCount=x*y;

  //allocate memory
  if(images!=NULL) delete [] images;
  images=new SDL_Rect[imageCount];

  //generate tiles, left to right, top to bottom
  int count=0;
  for(int i=0;i<y;i++){
    for(int j=0;j<x;j++){
      images[count].x=j*szX;
      images[count].y=i*szY;
      images[count].w=szX;
      images[count].h=szY;
      count++;
    }
  }
  return true;
}

bool CGraphic::createImages(char* fn){
  if(texture==NULL) return false;

  char str[256];
  char* tok;

  FILE* f=fopen(fn,"rt");
  if(f==NULL) return false;

  //get number of tiles
  fgets(str,256,f);
  imageCount=atoi(str);

  //allocate memory
  if(images!=NULL) delete [] images;
  images=new SDL_Rect[imageCount];

  //read tiles
  int count=0;
  for(int i=0;i<imageCount;i++){
    fgets(str,256,f);
    tok=strtok(str,",\r\n");
    images[count].x=atoi(tok);
    tok=strtok(NULL,",\r\n");
    images[count].y=atoi(tok);
    tok=strtok(NULL,",\r\n");
    images[count].w=atoi(tok);
    tok=strtok(NULL,",\r\n");
    images[count].h=atoi(tok);
    count++;
  }

  fclose(f);
  return true;
}

void CGraphic::render(SDL_Renderer* rend, int index, int posX, int posY, int w, int h){
  SDL_Rect r;
  r=images[index];
  r.x=posX;
  r.y=posY;
  if(w>0) r.w=w;
  if(h>0) r.h=h;
  SDL_RenderCopy(rend,texture,&images[index],&r);
}

int CGraphic::size(){
  return imageCount;
}
