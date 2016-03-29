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
  SDL_SetRenderDrawColor(display->renderer,58,58,58,255);
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
    font->render(posX+sz+4,posY,caption,1);
    font->fontSize=i;
  }

}

void CCheckbox::setDisplay(CDisplay* d){
  display=d;
}

void CCheckbox::setFocus(CActiveFocus* f){
  activeFocus=f;
}

void CCheckbox::setFont(CFont* f){
  font=f;
}

void CCheckbox::setSize(int s){
  sz = s;
}
