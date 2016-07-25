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

#include "CAbout.h"

CAbout::CAbout(){
  activeFocus = NULL;
  display = NULL;
  font = NULL;
}

CAbout::~CAbout(){
  activeFocus = NULL;
  display = NULL;
  font = NULL;
}

void CAbout::init(char* ver, char* bDate){

  butBack.posX=0;
  butBack.posY=00;
  butBack.szX=120;
  butBack.szY=50;
  butBack.setCaption("Back");
  version=ver;
  bdate=bDate;

}

int CAbout::logic(int mouseX, int mouseY, int mouseButton, bool mouseButton1){
  if(butBack.logic(mouseX, mouseY, mouseButton)) {
    return 1;
  }

  return 0;
}

void CAbout::render(){
  //always full screen
  SDL_Rect r;
  int fontSize=font->fontSize;

  //Clear the entire window
  SDL_RenderGetViewport(display->renderer, &r);
  SDL_SetRenderDrawColor(display->renderer, 255, 255, 255, 255);
  SDL_RenderFillRect(display->renderer, &r);

  //Draw the sidebar
  r.w=120;
  SDL_SetRenderDrawColor(display->renderer, 85, 98, 112, 255);
  SDL_RenderFillRect(display->renderer, &r);

  //Draw the buttons
  butBack.render();

  //Draw the information
  font->fontSize=20;
  font->render(150, 20, "Kojak Spectrum Viewer");
  font->fontSize=15;
  font->render(150, 42, version);
  font->render(150, 58, bdate);
  font->render(150, 74, "Copyright (C) 2016, Michael Hoopmann, Institute for Systems Biology");
  font->render(150, 90, "For more information, visit: http://kojak-ms.org");

  font->fontSize=fontSize;

}

void CAbout::setDisplay(CDisplay* d){
  display = d;
  butBack.setDisplay(d);
}

void CAbout::setFocus(CActiveFocus* f){
  activeFocus = f;
  butBack.setFocus(f);
}

void CAbout::setFont(CFont* f){
  font = f;
  butBack.setFont(f);
}

