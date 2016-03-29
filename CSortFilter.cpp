#include "CSortFilter.h"

CSortFilter::CSortFilter(){
  activeFocus = NULL;
  display = NULL;
  font = NULL;
  dt = NULL;
}

CSortFilter::~CSortFilter(){
  activeFocus = NULL;
  display = NULL;
  font = NULL;
  dt = NULL;
}

void CSortFilter::clear(){
  ddSort.clear();
}

void CSortFilter::init(CTable* t){
  butApply.posX=0;
  butApply.posY=0;
  butApply.szX=120;
  butApply.szY=50;

  butBack.posX=0;
  butBack.posY=50;
  butBack.szX=120;
  butBack.szY=50;

  butApply.setCaption("Apply");
  butBack.setCaption("Back");

  cbSortAsc.posX=310;
  cbSortAsc.posY=20;
  cbSortAsc.setSize(16);
  cbSortAsc.caption="Ascending";
  cbSortAsc.checked=false;

  dt=t;

  ddSort.posX=150;
  ddSort.posY=20;
  ddSort.szX=150;
  ddSort.szY=18;
  ddSort.szDrop=200;
  ddSort.fontSize=12;
  ddSort.addItem("none");

  size_t i;
  for(i=0;i<dt->size(true);i++){
    ddSort.addItem(dt->col(i).header);
    printf("Added: %s\n",&dt->col(i).header[0]);
  }

}

int CSortFilter::logic(int mouseX, int mouseY, int mouseButton, bool mouseButton1){
  if(butApply.logic(mouseX,mouseY,mouseButton)) {
    dt->sort(ddSort.getSelected(),!cbSortAsc.checked);
    return 0;
  }
  if(butBack.logic(mouseX,mouseY,mouseButton)) {
    return 1;
  }
  if(ddSort.logic(mouseX,mouseY,mouseButton,mouseButton1)){
    return 0;
  }
  if(cbSortAsc.logic(mouseX,mouseY,mouseButton)){
    return 0;
  }
  return 0;
}

void CSortFilter::render(){
  //always full screen
  SDL_Rect r;

  //Clear the entire window
  SDL_RenderGetViewport(display->renderer,&r);
  SDL_SetRenderDrawColor(display->renderer,255,255,255,255);
  SDL_RenderFillRect(display->renderer,&r);

  //Draw the sidebar
  r.w=120;
  SDL_SetRenderDrawColor(display->renderer,128,128,128,255);
  SDL_RenderFillRect(display->renderer,&r);

  //Draw the buttons
  butApply.render();
  butBack.render();

  //Draw the sort components
  ddSort.render();
  cbSortAsc.render();

}

void CSortFilter::setDisplay(CDisplay* d){
  display = d;
  butApply.setDisplay(d);
  butBack.setDisplay(d);
  cbSortAsc.setDisplay(d);
  ddSort.setDisplay(d);
}

void CSortFilter::setFocus(CActiveFocus* f){
  activeFocus = f;
  butApply.setFocus(f);
  butBack.setFocus(f);
  cbSortAsc.setFocus(f);
  ddSort.setFocus(f);
}

void CSortFilter::setFont(CFont* f){
  font = f;
  butApply.setFont(f);
  butBack.setFont(f);
  cbSortAsc.setFont(f);
  ddSort.setFont(f);
}