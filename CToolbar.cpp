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

  prevButton.posX=34;
  prevButton.posY=2;
  prevButton.szX=28;
  prevButton.szY=28;

  nextButton.posX=66;
  nextButton.posY=2;
  nextButton.szX=28;
  nextButton.szY=28;

  jumpBox.posX=98;
  jumpBox.posY=2;
  jumpBox.szX=256;
  jumpBox.szY=28;

  sfButton.posX=358;
  sfButton.posY=2;
  sfButton.szX=28;
  sfButton.szY=28;

  lockButton=false;

}

CToolbar::~CToolbar(){
  activeFocus = NULL;
  display = NULL;
  font    = NULL;
  input   = NULL;
}

void CToolbar::init(){
  prevButton.setCaption("<");
  nextButton.setCaption(">");
  sfButton.setCaption("*");
}

int CToolbar::logic(int mouseX, int mouseY, int mouseButton){
  if(openButton.logic(mouseX,mouseY,mouseButton)) return 1;
  if(prevButton.logic(mouseX,mouseY,mouseButton)) return 2;
  if(nextButton.logic(mouseX,mouseY,mouseButton)) return 3;
  if(jumpBox.logic(mouseX,mouseY,mouseButton)) return 4;
  if(sfButton.logic(mouseX,mouseY,mouseButton)) return 5;
  
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

  if(activeFocus->focus == &jumpBox){
    scanNumber = jumpBox.processInput();
    return 1;
  }
  return 0;
}
  
void CToolbar::render(){
  SDL_Rect r;

  //Draw background
  if(szX<0) display->getWindowSize(r.w,r.h);
  else r.w=szX;
  r.h=szY;
  r.x=posX;
  r.y=posY;
  SDL_SetRenderDrawColor(display->renderer,color.r,color.g,color.b,255);
  SDL_RenderFillRect(display->renderer,&r);

  openButton.render();
  prevButton.render();
  nextButton.render();
  jumpBox.render();
  sfButton.render();

}

void CToolbar::setDisplay(CDisplay* d){
  display=d;
  color=display->pal.buttons[0];
  openButton.setDisplay(d);
  prevButton.setDisplay(d);
  nextButton.setDisplay(d);
  jumpBox.setDisplay(d);
  sfButton.setDisplay(d);
}

void CToolbar::setFocus(CActiveFocus* f){
  activeFocus=f;
  openButton.setFocus(f);
  prevButton.setFocus(f);
  nextButton.setFocus(f);
  jumpBox.setFocus(f);
  sfButton.setFocus(f);
}

void CToolbar::setFont(CFont* f){
  font=f;
  openButton.setFont(f);
  prevButton.setFont(f);
  nextButton.setFont(f);
  jumpBox.setFont(f);
  sfButton.setFont(f);
}

void CToolbar::setInput(CInput* i){
  input=i;
  jumpBox.setInput(i);
}
