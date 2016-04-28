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

#include "CButton.h"

CButton::CButton(){
  activeFocus = NULL;
  display = NULL;
  font    = NULL;
  image = NULL;

  active=true;
  lockButton=false;
  highlight=false;
  imgIndex=-1;
  posX=0;
  posY=0;
  szFont=16;
  szX=0;
  szY=0;
  caption="";
  textWidth=0;
  textHeight=0;
}

CButton::~CButton(){
  activeFocus = NULL;
  display = NULL;
  font    = NULL;
  image   = NULL;
}

bool CButton::logic(int mouseX, int mouseY, int mouseButton){
  if(!active) return false;
  if(mouseX>=posX && mouseX<=(posX+szX) && mouseY>=posY && mouseY<=(posY+szY)) {
    highlight=true;
    if(mouseButton==1 && !lockButton){
      activeFocus->focus=this;
      lockButton=true;
      return true;
    }
  } else {
    highlight=false;
  }
  if(lockButton && mouseButton==0) lockButton=false;

  return false; 
}
  
void CButton::render(){
  SDL_Rect r;
  int fontSize = font->fontSize;
  font->fontSize=szFont;

  //Draw background
  r.h=szY;
  r.w=szX;
  r.x=posX;
  r.y=posY;
  if(active){
    if(highlight) {
      SDL_SetRenderDrawColor(display->renderer,colors[1].r,colors[1].g,colors[1].b,255);
      SDL_RenderFillRect(display->renderer,&r);
      SDL_SetRenderDrawColor(display->renderer,colors[2].r,colors[2].g,colors[2].b,255);
      SDL_RenderDrawRect(display->renderer,&r);
    } else {
      SDL_SetRenderDrawColor(display->renderer,colors[0].r,colors[0].g,colors[0].b,255);
      SDL_RenderFillRect(display->renderer,&r);
    }
  } else {
    SDL_SetRenderDrawColor(display->renderer,colors[4].r,colors[4].g,colors[4].b,255);
    SDL_RenderFillRect(display->renderer,&r);
  }

  //Draw button
  if(imgIndex>-1){
    r=image->getRect(imgIndex);
    if(r.w>=szX-2){
      r.w=szX-2;
      r.h=szX-2;
    }
    if(r.h>=szY-2){
      r.w=szY-2;
      r.h=szY-2;
    }
    r.x=posX+(szX-r.w)/2;
    r.y=posY+(szY-r.h)/2;
    image->render(display->renderer,imgIndex,r.x,r.y,r.w,r.h);
  } else {
    r.x=posX+(szX-textWidth)/2;
    r.y=posY+(szY-textHeight)/2;
    font->render(r.x,r.y,caption,txtColors[0]);
  }

  font->fontSize = fontSize;

}

void CButton::setCaption(char* str){
  int fontSize=font->fontSize;
  caption=str;
  font->fontSize=szFont;
  textWidth=font->getStringWidth(str);
  textHeight=font->getFontHeight();
  font->fontSize=fontSize;
}

void CButton::setCaption(string str){
  setCaption(&str[0]);
}

void CButton::setDisplay(CDisplay* d){
  display=d;
  for(size_t i=0;i<5;i++) colors[i]=display->pal.buttons[i];
  txtColors[0]=display->pal.txtButton[0];
  txtColors[1]=display->pal.txtButton[1];
}

void CButton::setFocus(CActiveFocus* f){
  activeFocus=f;
}

void CButton::setFont(CFont* f){
  font=f;
}

void CButton::setGfx(CGfxCollection* g) {
  image=g->icons;
}

void CButton::setImage(int index) {
  if(index<0) imgIndex=-1;
  if(image==NULL) imgIndex=-1;
  if(index>=image->size()) imgIndex=-1;
  imgIndex=index;
}



