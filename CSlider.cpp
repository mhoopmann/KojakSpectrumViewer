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
    if(mouseButton1 && mouseX>=posX && mouseX<=posX+szX && mouseY>=posY && mouseY<=posY+szY){
      activeFocus->focus=this;
      sliderLock=true;
      if(vertical) lastMouse=mouseX;
      else lastMouse=mouseY;
      return 0;
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
