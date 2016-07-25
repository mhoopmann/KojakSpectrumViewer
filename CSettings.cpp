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

#include "CSettings.h"

CSettings::CSettings(){
  activeFocus = NULL;
  display = NULL;
  font = NULL;
  input = NULL;

  lineWidth=1;
  tol = 0.01;
  tolUnit=0;
}

CSettings::~CSettings(){
  activeFocus = NULL;
  display = NULL;
  font = NULL;
  input = NULL;
}

void CSettings::init(){
  butApply.posX=0;
  butApply.posY=0;
  butApply.szX=120;
  butApply.szY=50;

  butCancel.posX=0;
  butCancel.posY=50;
  butCancel.szX=120;
  butCancel.szY=50;

  butApply.setCaption("Apply");
  butCancel.setCaption("Cancel");

  ddTolUnit.posX=395;
  ddTolUnit.posY=20;
  ddTolUnit.szX=55;
  ddTolUnit.szDrop=40;
  ddTolUnit.setFontSize(16);
  ddTolUnit.addItem("Da");
  ddTolUnit.addItem("ppm");
  ddTolUnit.selected=(int)tolUnit;

  ebTol.posX=290;
  ebTol.posY=20;
  ebTol.szX=100;
  ebTol.setFontSize(16);
  ebTol.numeric=true;
  ebTol.setCaption(tol);

  pmWidth.posX=290;
  pmWidth.posY=50;
  pmWidth.szX=100;
  pmWidth.setFontSize(16);
  pmWidth.value=1;
  pmWidth.min=1;
  pmWidth.max=3;

}

int CSettings::logic(int mouseX, int mouseY, int mouseButton, bool mouseButton1){
  if(butApply.logic(mouseX, mouseY, mouseButton)) {
    lineWidth=pmWidth.value;
    ebTol.getCaption(NULL, &tol, NULL);
    tolUnit=(char)ddTolUnit.selected;
    return 2;
  }
  if(butCancel.logic(mouseX, mouseY, mouseButton)) {
    return 1;
  }
  if(ddTolUnit.logic(mouseX, mouseY, mouseButton, mouseButton1)){
    return 0;
  }
  if(ebTol.logic(mouseX, mouseY, mouseButton)) return 0;
  if(pmWidth.logic(mouseX,mouseY,mouseButton,mouseButton1)) return 0;

  if(activeFocus->focus == &ebTol){
    ebTol.processInput();
  }

  return 0;
}

void CSettings::render(){
  //always full screen
  SDL_Rect r;
  int fontSize = font->fontSize;
  font->fontSize = 20;

  //Clear the entire window
  SDL_RenderGetViewport(display->renderer, &r);
  SDL_SetRenderDrawColor(display->renderer, 255, 255, 255, 255);
  SDL_RenderFillRect(display->renderer, &r);

  //Draw the sidebar
  r.w=120;
  SDL_SetRenderDrawColor(display->renderer, 85, 98, 112, 255);
  SDL_RenderFillRect(display->renderer, &r);

  //Draw the buttons
  butApply.render();
  butCancel.render();

  //Draw the Parameters
  font->render(150, 50, "Line Weight:");
  pmWidth.render();

  font->render(150, 20, "Mass Tolerance:");
  ebTol.render();
  ddTolUnit.render(); 

  font->fontSize = fontSize;

}

void CSettings::setDisplay(CDisplay* d){
  display = d;
  butApply.setDisplay(d);
  butCancel.setDisplay(d);
  ebTol.setDisplay(d);
  ddTolUnit.setDisplay(d);
  pmWidth.setDisplay(d);
}

void CSettings::setFocus(CActiveFocus* f){
  activeFocus = f;
  butApply.setFocus(f);
  butCancel.setFocus(f);
  ebTol.setFocus(f);
  ddTolUnit.setFocus(f);
  pmWidth.setFocus(f);
}

void CSettings::setFont(CFont* f){
  font = f;
  butApply.setFont(f);
  butCancel.setFont(f);
  ebTol.setFont(f);
  ddTolUnit.setFont(f);
  pmWidth.setFont(f);
}

void CSettings::setInput(CInput* i){
  input = i;
  ebTol.setInput(i);
}

