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
    font->setFontSize(16);
    font->render(r.x,r.y,caption);
  }

}

void CButton::setCaption(char* str){
  caption=str;
  font->setFontSize(16);
  textWidth=font->getStringWidth(str);
  textHeight=font->getFontHeight();
}

void CButton::setCaption(string str){
  setCaption(&str[0]);
}

void CButton::setDisplay(CDisplay* d){
  display=d;
  for(size_t i=0;i<5;i++) colors[i]=display->pal.buttons[i];
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



