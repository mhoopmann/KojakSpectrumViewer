#include "CToolbar.h"

CToolbar::CToolbar(){
  activeFocus = NULL;
  display = NULL;
  font    = NULL;
  input   = NULL;

  posX = 0;
  posY = 0;
  szX  = -1;
  szY  = 32;
  scanNumber = -1;

  openButton.posX=2;
  openButton.posY=2;
  openButton.szX=28;
  openButton.szY=28;

  addSeparator(34,2);

  prevButton.posX=38;
  prevButton.posY=2;
  prevButton.szX=28;
  prevButton.szY=28;

  nextButton.posX=70;
  nextButton.posY=2;
  nextButton.szX=28;
  nextButton.szY=28;

  addSeparator(102,2);

  sfButton.posX=106;
  sfButton.posY=2;
  sfButton.szX=28;
  sfButton.szY=28;

  setButton.posX=138;
  setButton.posY=2;
  setButton.szX=28;
  setButton.szY=28;

  addSeparator(170, 2);

  aboutButton.posX=174;
  aboutButton.posY=2;
  aboutButton.szX=28;
  aboutButton.szY=28;

  lockButton=false;

}

CToolbar::~CToolbar(){
  activeFocus = NULL;
  display = NULL;
  font    = NULL;
  input   = NULL;
}

void CToolbar::addSeparator(int posX,int posY){
  SDL_Rect r;
  r.x=posX;
  r.y=posY;
  r.w=2;
  r.h=28;
  separators.push_back(r);
}

void CToolbar::init(){
  openButton.setImage(0);
  prevButton.setImage(2);
  nextButton.setImage(1);
  sfButton.setImage(3);
  aboutButton.setImage(5);
  setButton.setImage(6);
}

int CToolbar::logic(int mouseX, int mouseY, int mouseButton){
  if(openButton.logic(mouseX,mouseY,mouseButton)) return 1;
  if(prevButton.logic(mouseX,mouseY,mouseButton)) return 2;
  if(nextButton.logic(mouseX,mouseY,mouseButton)) return 3;
  if(aboutButton.logic(mouseX,mouseY,mouseButton)) return 4;
  if(sfButton.logic(mouseX,mouseY,mouseButton)) return 5;
  if(setButton.logic(mouseX, mouseY, mouseButton)) return 7;
  
  int x,y;
  if(szX<0) display->getWindowSize(x,y);
  else x=szX;
  if(mouseX>=posX && mouseX<=(posX+x) && mouseY>=posY && mouseY<=(posY+szY)) {
    if(mouseButton==1 && !lockButton){
      activeFocus->focus=this;
      lockButton=true;
      return 6;
    }  
  }
  if(lockButton && mouseButton==0) lockButton=false;
  return 0; 
}

int CToolbar::processInput(){

  //0 = no input
  //1 = jumpBox was processed

  //if(activeFocus->focus == &jumpBox){
  //  scanNumber = jumpBox.processInput();
  //  return 1;
  //}
  return 0;
}
  
void CToolbar::render(){
  SDL_Rect r;
  size_t i;

  //Draw background
  if(szX<0) display->getWindowSize(r.w,r.h);
  else r.w=szX;
  r.h=szY;
  r.x=posX;
  r.y=posY;
  SDL_SetRenderDrawColor(display->renderer,colors[0].r,colors[0].g,colors[0].b,255);
  SDL_RenderFillRect(display->renderer,&r);

  //Draw separators
  SDL_SetRenderDrawColor(display->renderer,colors[1].r,colors[1].g,colors[1].b,255);
  for(i=0;i<separators.size();i++){
    SDL_RenderFillRect(display->renderer,&separators[i]);
  }

  openButton.render();
  prevButton.render();
  nextButton.render();
  sfButton.render();
  aboutButton.render();
  setButton.render();

}

void CToolbar::setDisplay(CDisplay* d){
  display=d;
  colors[0]=display->pal.toolbar[0];
  colors[1]=display->pal.toolbar[1];
  openButton.setDisplay(d);
  prevButton.setDisplay(d);
  nextButton.setDisplay(d);
  sfButton.setDisplay(d);
  aboutButton.setDisplay(d);
  setButton.setDisplay(d);
}

void CToolbar::setFocus(CActiveFocus* f){
  activeFocus=f;
  openButton.setFocus(f);
  prevButton.setFocus(f);
  nextButton.setFocus(f);
  sfButton.setFocus(f);
  aboutButton.setFocus(f);
  setButton.setFocus(f);
}

void CToolbar::setFont(CFont* f){
  font=f;
  openButton.setFont(f);
  prevButton.setFont(f);
  nextButton.setFont(f);
  sfButton.setFont(f);
  aboutButton.setFont(f);
  setButton.setFont(f);
}

void CToolbar::setGfx(CGfxCollection* g){
  openButton.setGfx(g);
  prevButton.setGfx(g);
  nextButton.setGfx(g);
  sfButton.setGfx(g);
  aboutButton.setGfx(g);
  setButton.setGfx(g);
}

void CToolbar::setInput(CInput* i){
  input=i;
}
