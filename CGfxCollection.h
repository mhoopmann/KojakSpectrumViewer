#ifndef _CGFXCOLLECTION_H
#define _CGFXCOLLECTION_H

#include "CGraphic.h"

class CGfxCollection{
public:
  CGfxCollection();
  ~CGfxCollection();

  CGraphic* icons;
  bool loadGfx(char* fn, SDL_Renderer* rend);

private:

};

#endif