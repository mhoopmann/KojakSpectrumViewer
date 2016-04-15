#include "CGfxCollection.h"

CGfxCollection::CGfxCollection(){
  icons=NULL;
}

CGfxCollection::~CGfxCollection(){
  if(icons!=NULL) delete icons;
}

bool CGfxCollection::loadGfx(char* fn, SDL_Renderer *rend){
  if(icons!=NULL) delete icons;
  icons = new CGraphic;
  icons->loadTexture(fn,rend,false,true,255,0,255);
  icons->createImages(24,24,168,24);

  return true;
}