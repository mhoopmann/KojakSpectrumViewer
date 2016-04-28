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

#include "CSlider.h"

CSlider::CSlider(){
  activeFocus=NULL;
  display=NULL;
  sliderLock=false;
  lastMouse=0;
  vertical=true;
}

CSlider::~CSlider(){
  activeFocus=NULL;
  display=NULL;
}

bool CSlider::checkLocked(){
  return sliderLock;
}

int CSlider::logic(int mouseX, int mouseY, int mouseButton, bool mouseButton1){
  int val;
  if(sliderLock){
    if(mouseButton1){
      if(vertical){
        val=mouseX-lastMouse;
        posX+=val;
        lastMouse=mouseX;
      } else {
        val=mouseY-lastMouse;
        posY+=val;
        lastMouse=mouseY;
      }
      return val;
    } else {
      sliderLock=false;
      return 10000;
    }
  } else {
    if(mouseX>=posX && mouseX<=posX+szX && mouseY>=posY && mouseY<=posY+szY){
      if(vertical) SDL_SetCursor(display->cursors[2]);
      else SDL_SetCursor(display->cursors[3]);

      if(mouseButton1){
        activeFocus->focus=this;
        sliderLock=true;
        if(vertical) lastMouse=mouseX;
        else lastMouse=mouseY;  
      }
      return 0;

    } else {
      SDL_SetCursor(display->cursors[0]);
    }
  }

  return 10000;
}

bool CSlider::render(){
  SDL_Rect r;
  r.x=posX;
  r.y=posY;
  r.h=szY;
  r.w=szX;
  SDL_SetRenderDrawColor(display->renderer,color.r,color.g,color.b,255);
  SDL_RenderFillRect(display->renderer,&r);
  return true;
}

void CSlider::setDisplay(CDisplay* d){
  display = d;
  color = display->pal.buttons[0];
}

void CSlider::setFocus(CActiveFocus* f){
  activeFocus = f;
}
