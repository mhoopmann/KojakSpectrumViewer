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

void CFilterManager::clear(){
  vChosen.clear();
}

void CFilterManager::init(CTable* t){
  item.posX=0;
  item.posY=30;
  item.szX=120;
  item.szDrop=200;
  item.setFontSize(16);

  filter.posX=124;
  filter.posY=30;
  filter.szX=120;
  filter.szDrop=60;
  filter.setFontSize(16);
  filter.fixLayout();

  valueA.posX = 248;
  valueA.posY = 30;
  valueA.szX = 120;
  valueA.setFontSize(16);

  valueB.posX = 372;
  valueB.posY = 30;
  valueB.szX = 120;
  valueB.setFontSize(16);
  valueB.numeric=true;
  valueB.active=false;

  butAdd.posX = 496;
  butAdd.posY = 30;
  butAdd.szX = 20;
  butAdd.szY = 20;
  butAdd.setCaption("+");

  kvFilter f;
  size_t i;
  vAvail.clear();
  item.clear();
  for(i=0; i<t->size(true); i++){
    f.colID=t->col(i).header;
    f.type=t->col(i).dataType;
    vAvail.push_back(f);
    item.addItem(t->col(i).header);
  }

  if(item.size()>0){
    item.selected=0;
    filter.clear();
    if(vAvail[0].type<2){
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
  }

}

int CFilterManager::logic(int mouseX, int mouseY, int mouseButton, bool mouseButton1){
  //this object is in a viewport. Check logic relative to the viewport
  mouseX -= posX;
  mouseY -= posY;
  size_t i;

  switch(item.logic(mouseX, mouseY, mouseButton, mouseButton1)){
  case 1:
    return 1;
  case 2:
    filter.clear();
    if(vAvail[item.selected].type<2){
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
    if(vAvail[item.selected].type<2){
      if(filter.selected==2) valueB.active=true;
      else valueB.active=false;
    }
    return 1;
  default:
    break;
  }

  if(valueA.logic(mouseX,mouseY,mouseButton)) return 4;
  if(valueB.logic(mouseX, mouseY, mouseButton)) return 4;

  if(butAdd.logic(mouseX, mouseY, mouseButton)) {
    kvFilter f;
    f=vAvail[item.selected];
    if(f.type<2) f.filter = (char)filter.selected;
    else f.filter=(char)filter.selected + 3;
    valueA.getCaption(&f.iLow, &f.dLow, &f.sLow);
    valueB.getCaption(&f.iHigh, &f.dHigh, &f.sHigh);
    vChosen.push_back(f);
    CButton b;
    vButDel.push_back(b);
    i=(int)vButDel.size()-1;
    vButDel[i].setDisplay(display);
    vButDel[i].setFocus(activeFocus);
    vButDel[i].setFont(font);
    vButDel[i].setCaption("-");
    vButDel[i].posX=0;
    vButDel[i].posY=110+(int)i*25;
    vButDel[i].szX=18;
    vButDel[i].szY=18;
    return 1;
  }

  for(i=0; i<vButDel.size(); i++){
    if(vButDel[i].logic(mouseX, mouseY, mouseButton)){
      vChosen.erase(vChosen.begin()+i);
      vButDel.pop_back();
      return 1;
    }
  }
  
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
  size_t i;
  SDL_Rect r;
  char str[256];
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
  font->fontSize=20;
  font->render(0, 0, "Filter PSMs:", 1);
  font->render(0, 80, "Applied Filters:", 1);

  //Display all filter items
  font->fontSize=18;
  for(i=0; i<vChosen.size(); i++){
    vButDel[i].render();
    if(vChosen[i].filter==2) {
      if(vChosen[i].type==0) sprintf(str, "%s: between %d and %d", &vChosen[i].colID[0], vChosen[i].iLow, vChosen[i].iHigh);
      else sprintf(str, "%s: between %lf and %lf", &vChosen[i].colID[0], vChosen[i].dLow, vChosen[i].dHigh);
    } else {
      if(vChosen[i].type==0) {
        if(vChosen[i].filter==0) sprintf(str, "%s: greater than %d", &vChosen[i].colID[0], vChosen[i].iLow);
        else sprintf(str, "%s: less than %d", &vChosen[i].colID[0], vChosen[i].iLow);
      } else if(vChosen[i].type==1) {
        if(vChosen[i].filter==0)  sprintf(str, "%s: greater than %lf", &vChosen[i].colID[0], vChosen[i].dLow);
        else sprintf(str, "%s: less than %lf", &vChosen[i].colID[0], vChosen[i].dLow);
      } else {
        if(vChosen[i].filter==3) sprintf(str, "%s: contains %s", &vChosen[i].colID[0], &vChosen[i].sLow[0]);
        else sprintf(str, "%s: is exactly %s", &vChosen[i].colID[0], &vChosen[i].sLow[0]);
      }
    }
    font->render(30,110+(int)i*25, str, 1);
  }

  r.x=0;
  r.y=65;
  r.w=vp.w-20;
  r.h=3;
  SDL_SetRenderDrawColor(display->renderer, 85, 98, 112, 255);
  SDL_RenderFillRect(display->renderer, &r);

  //Render the drop down menus
  item.render();
  filter.render();

  //Render the edit boxes
  valueA.render();
  valueB.render();

  //Render the button;
  butAdd.render();

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
  butAdd.setDisplay(d);
}

void CFilterManager::setFocus(CActiveFocus* f){
  activeFocus = f;
  filter.setFocus(f);
  item.setFocus(f);
  valueA.setFocus(f);
  valueB.setFocus(f);
  butAdd.setFocus(f);
}

void CFilterManager::setFont(CFont* f){
  font = f;
  filter.setFont(f);
  item.setFont(f);
  valueA.setFont(f);
  valueB.setFont(f);
  butAdd.setFont(f);
}

void CFilterManager::setInput(CInput* i){
  input = i;
  valueA.setInput(i);
  valueB.setInput(i);
}
