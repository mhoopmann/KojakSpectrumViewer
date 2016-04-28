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

#include "CCheckbox.h"

CCheckbox::CCheckbox(){
  activeFocus = NULL;
  display = NULL;
  font = NULL;
  active = true;
  checked = false;
  sz = 8;
  posX = 0;
  posY = 0;
  caption.clear();
}

CCheckbox::~CCheckbox(){
  activeFocus = NULL;
  display = NULL;
  font = NULL;
}

int CCheckbox::getCaptionWidth(){
  int fontSize=font->fontSize;
  font->fontSize=sz;
  int i = font->getStringWidth(caption);
  font->fontSize=fontSize;
  return i;
}

bool CCheckbox::logic(int mouseX, int mouseY, int mouseButton){
  if(mouseButton==1){
    if(mouseX>=posX && mouseX<=(posX+sz) && mouseY>=posY && mouseY<=(posY+sz) && active){
      activeFocus->focus=this;
      if(checked) checked = false;
      else checked = true;
      return true;
    }
  }
  return false;
}

void CCheckbox::render(){
  SDL_Rect r;

  //Draw background
  r.h=sz;
  r.w=sz;
  r.x=posX;
  r.y=posY;
  //SDL_SetRenderDrawColor(display->renderer,25,25,25,255);
  //SDL_RenderFillRect(display->renderer,&r);

  //Draw border
  SDL_SetRenderDrawColor(display->renderer,color.r,color.g,color.b,255);
  SDL_RenderDrawRect(display->renderer,&r);

  //Draw checkbox
  if(checked){
    r.h-=4;
    r.w-=4;
    r.x+=2;
    r.y+=2;
    SDL_RenderFillRect(display->renderer,&r);
  }

  //Draw caption
  if(caption.size()>0){
    int i=font->fontSize;
    font->fontSize=sz;
    font->render(posX+sz+4,posY,caption,txtColor);
    font->fontSize=i;
  }

}

void CCheckbox::setDisplay(CDisplay* d){
  display=d;
  color=display->pal.checkbox;
  txtColor=display->pal.txtCheckbox;
}

void CCheckbox::setFocus(CActiveFocus* f){
  activeFocus=f;
}

void CCheckbox::setFont(CFont* f){
  font=f;
}

