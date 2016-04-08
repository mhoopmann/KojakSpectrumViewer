#include "CFilterManager.h"

CFilterManager::CFilterManager(){
  activeFocus = NULL;
  display = NULL;
  font = NULL;
  input = NULL;

  posX = 0;
  posY = 0;
}

CFilterManager::~CFilterManager(){
  activeFocus = NULL;
  display = NULL;
  font = NULL;
  input = NULL;
}

void CFilterManager::init(CTable* t){
  item.posX=0;
  item.posY=20;
  item.szX=120;
  item.szDrop=200;
  item.setFontSize(14);

  filter.posX=124;
  filter.posY=20;
  filter.szX=120;
  filter.szDrop=56;
  filter.setFontSize(14);
  filter.fixLayout();

  valueA.posX = 248;
  valueA.posY = 20;
  valueA.szX = 120;
  valueA.setFontSize(14);

  valueB.posX = 372;
  valueB.posY = 20;
  valueB.szX = 120;
  valueB.setFontSize(14);
  valueB.numeric=true;
  valueB.active=false;

  kvFilter f;
  size_t i;
  for(i=0; i<t->size(true); i++){
    f.colID=t->col(i).header;
    if(t->col(i).dataType==2) f.type=1;
    else f.type=0;
    vAvail.push_back(f);
    item.addItem(t->col(i).header);
  }

  if(item.size()>0){
    item.selected=0;
    if(vAvail[0].type==0){
      filter.clear();
      filter.addItem("greater than");
      filter.addItem("less than");
      filter.addItem("between");
      filter.selected=0;
      valueA.active=true;
      valueB.active=false; 
    } else {
      filter.clear();
      filter.addItem("contains");
      filter.addItem("is exactly");
      filter.selected=0;
      valueA.numeric=false;
      valueB.active=false;
    }
  }

}

int CFilterManager::logic(int mouseX, int mouseY, int mouseButton, bool mouseButton1){
  //this object is in a viewport. Check logic relative to the viewport
  mouseX -= posX;
  mouseY -= posY;

  switch(item.logic(mouseX, mouseY, mouseButton, mouseButton1)){
  case 1:
    return 1;
  case 2:
    filter.clear();
    if(vAvail[item.selected].type==0){
      filter.addItem("greater than");
      filter.addItem("less than");
      filter.addItem("between");
      valueA.numeric=true;
      valueB.active=false;
    } else {
      filter.addItem("contains");
      filter.addItem("is exactly");
      valueA.numeric=false;
      valueB.active=false;
    }
    filter.selected=0;
    valueA.clear();
    valueB.clear();
    return 1;
  default:
    break;
  }

  switch(filter.logic(mouseX, mouseY, mouseButton, mouseButton1)){
  case 1:
    return 1;
  case 2:
    if(vAvail[item.selected].type==0){
      if(filter.selected==2) valueB.active=true;
      else valueB.active=false;
    }
    return 1;
  default:
    break;
  }

  if(valueA.logic(mouseX,mouseY,mouseButton)) return 4;
  if(valueB.logic(mouseX, mouseY, mouseButton)) return 4;

  return 0;
}

int CFilterManager::processInput(){

  //0 = no input
  //1 = valueA was processed
  if(activeFocus->focus == &valueA){
    fflush(stdout);
    valueA.processInput();
    return 1;
  }
  if(activeFocus->focus == &valueB){
    fflush(stdout);
    valueB.processInput();
    return 1;
  }
  return 0;
}

void CFilterManager::render(){
  fflush(stdout);
  SDL_Rect origVP;
  SDL_Rect vp;
  //SDL_Rect r;
  //char str[64];
  int fontSize = font->fontSize;

  SDL_RenderGetViewport(display->renderer, &origVP);

  //Render from here to the end of the window;
  vp = origVP;
  vp.x = posX;
  vp.y = posY;
  vp.w = origVP.w - posX;
  vp.h = origVP.h - posY;
  SDL_RenderSetViewport(display->renderer, &vp);

  //Display the text
  font->fontSize=16;
  font->render(0, 0, "Filter PSMs: (currently disabled pending code revisions)", 1);

  //Render the drop down menus
  item.render();
  filter.render();

  //Render the edit boxes
  valueA.render();
  valueB.render();

  //reset viewport to full window
  SDL_RenderSetViewport(display->renderer, &origVP);

  font->fontSize = fontSize;
}

void CFilterManager::setDisplay(CDisplay* d){
  display = d;
  filter.setDisplay(d);
  item.setDisplay(d);
  valueA.setDisplay(d);
  valueB.setDisplay(d);
}

void CFilterManager::setFocus(CActiveFocus* f){
  activeFocus = f;
  filter.setFocus(f);
  item.setFocus(f);
  valueA.setFocus(f);
  valueB.setFocus(f);
}

void CFilterManager::setFont(CFont* f){
  font = f;
  filter.setFont(f);
  item.setFont(f);
  valueA.setFont(f);
  valueB.setFont(f);
}

void CFilterManager::setInput(CInput* i){
  input = i;
  valueA.setInput(i);
  valueB.setInput(i);
}
