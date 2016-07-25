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
  icons->createImages(24,24,192,24);

  return true;
}