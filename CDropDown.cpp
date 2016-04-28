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

#include "CDropDown.h"

CDropDown::CDropDown(){
  activeFocus = NULL;
  display = NULL;
  font = NULL;
  active = true;
  dropped = false;
  posX = 0;
  posY = 0;
  szX = 0;
  szY = 0;
  szDrop = 0;
  fontSize = 12;
  selected = -1;
  highlight=-1;

  showScrollbarV=false;
  scrollLockV=false;
  scrollOffsetV=0;
}

CDropDown::~CDropDown(){
  activeFocus = NULL;
  display = NULL;
  font = NULL;
}

void CDropDown::addItem(char* str){
  string s=str;
  addItem(s);
}

void CDropDown::addItem(string& str){
  items.push_back(str);
  fixLayout();
}

void CDropDown::clear(){
  items.clear();
  dropped=false;
  selected=-1;
  highlight=-1;
}

void CDropDown::fixLayout(){
  int contentSize;
  int scrollTrackSpace;
  int scrollThumbSpace;
  int viewSize;
  
  //Vertical Scrolling
  viewSize = szDrop-2;
  contentSize = (int)items.size()*(fontSize+3); //size of entire text content
  if(viewSize>0 && contentSize>viewSize) {
    showScrollbarV=true;
    
    thumbHeightV = viewSize*viewSize/contentSize;
    if(thumbHeightV<8) thumbHeightV=8;
    thumbMaxV = viewSize-thumbHeightV;

    scrollTrackSpace = contentSize-viewSize;
    scrollThumbSpace = viewSize-thumbHeightV;
    scrollJumpV = (double)scrollTrackSpace/scrollThumbSpace;
    scrollOffsetV = 0;
  } else showScrollbarV=false;
 
}

string CDropDown::getSelected(){
  if(selected<0) {
    string s;
    s.clear();
    return s;
  }
  return items[selected];
}

int CDropDown::logic(int mouseX, int mouseY, int mouseButton, bool mouseButton1){

  if(!active) return 0;

  if(mouseButton1 && dropped && showScrollbarV){
    //grabbed scrollbar
    if(mouseX>=posX+szX-8 && mouseX<=posX+szX-2 && mouseY>=posY+szY+scrollOffsetV+1 && mouseY<=posY+szY+scrollOffsetV+thumbHeightV+1){
      if(scrollLockV){
        scrollOffsetV+=(mouseY-lastMouseY);
        if(scrollOffsetV>thumbMaxV) scrollOffsetV=thumbMaxV;
        if(scrollOffsetV<0) scrollOffsetV=0;
        lastMouseY=mouseY;
      } else {
        scrollLockV=true;
        lastMouseY=mouseY;
      }
      return 1;
    } else if(scrollLockV){
      scrollOffsetV+=(mouseY-lastMouseY);
      if(scrollOffsetV>thumbMaxV) scrollOffsetV=thumbMaxV;
      if(scrollOffsetV<0) scrollOffsetV=0;
      lastMouseY=mouseY;
      return 1;
    }
  }

  //release scrollbar
  if(mouseButton==1 && scrollLockV){ 
    scrollLockV=false;
    return 1;
  }

  //selected an item
  if(mouseButton==1 && dropped && mouseX>=posX && mouseX<=(posX+szX-10) && mouseY>=posY+szY+1 && mouseY<=posY+szY+szDrop-1){
    int i=mouseY-posY-szY+(int)(scrollOffsetV*scrollJumpV);
    i/=(fontSize+3);
    if(i<(int)items.size()) selected = i;
    dropped=false;
    return 2;
  } 

  //clicked drop button
  if(button.logic(mouseX,mouseY,mouseButton)){
    if(dropped) dropped=false;
    else dropped=true;
    return 1;
  }

  //Check for highlight
  if(dropped && mouseX>=posX && mouseX<=(posX+szX-10) && mouseY>=posY+szY+1 && mouseY<=posY+szY+szDrop-1){
    int i=mouseY-posY-szY+(int)(scrollOffsetV*scrollJumpV);
    i/=(fontSize+3);
    if(i<(int)items.size()) highlight = i;
  } else {
    highlight=-1;
  }

  //if mouse is clicked anywhere in space, but there is no function, just do nothing
  if(mouseButton1 && mouseX>=posX && mouseX<=posX+szX){
    if(dropped && mouseY>=posY && mouseY<=posY+szY+szDrop) return 1;
    else if(mouseY>=posY && mouseY<=posY+szY) return 1;
  }

  scrollLockV=false;
  if(mouseButton1 && dropped) dropped=false; //mouse clicked anywhere else closes drop.

  return 0;
}

void CDropDown::render(){
  SDL_Rect r,r2;
  SDL_Rect origVP;
  SDL_Rect vp;
  int f=font->fontSize;
  font->fontSize=fontSize;

  SDL_RenderGetViewport(display->renderer,&origVP);

  //Draw background
  r.h=szY;
  r.w=szX;
  r.x=posX;
  r.y=posY;
  SDL_SetRenderDrawColor(display->renderer,255,255,255,255);
  SDL_RenderFillRect(display->renderer,&r);

  //Draw border
  SDL_SetRenderDrawColor(display->renderer,colors[0].r,colors[0].g,colors[0].b,255);
  SDL_RenderDrawRect(display->renderer,&r);

  //Draw button
  button.posX=posX+szX-szY;
  button.posY=posY;
  button.szX=szY;
  button.szY=szY;
  button.render();

  //Draw text
  if(selected>-1) font->render(posX+4,posY+2,items[selected],txtColor);
  
  //Draw list
  if(dropped){
    vp.h=szDrop;
    vp.w=szX;
    vp.x=posX+origVP.x;
    vp.y=posY+szY+origVP.y;
    SDL_RenderSetViewport(display->renderer,&vp);

    r.x=0;
    r.y=0;
    r.w=szX;
    r.h=szDrop;
    SDL_SetRenderDrawColor(display->renderer,255,255,255,255);
    SDL_RenderFillRect(display->renderer,&r);

    //Draw border
    SDL_SetRenderDrawColor(display->renderer,colors[0].r,colors[0].g,colors[0].b,255);
    SDL_RenderDrawRect(display->renderer,&r);

    r.x=4;
    r.y=1-(int)(scrollOffsetV*scrollJumpV);
    for(size_t i=0;i<items.size();i++){
      if(r.y+(fontSize+3)<0) {
        r.y+=(fontSize+3);
        continue;
      }
      if(highlight==(int)i){
        r2.x=1;
        r2.y=r.y;
        r2.w=szX-2;
        r2.h=fontSize+3;
        SDL_SetRenderDrawColor(display->renderer,colors[1].r,colors[1].g,colors[1].b,255);
        SDL_RenderFillRect(display->renderer,&r2);
      }
      font->render(r.x,r.y+1,items[i],txtColor);
      r.y+=(fontSize+3);
    }

    //Draw scrollbar
    if(showScrollbarV){
      r.x=szX-10;
      r.y=1;
      r.w=9;
      r.h=szDrop-2;
      SDL_SetRenderDrawColor(display->renderer,255,255,255,255);
      SDL_RenderFillRect(display->renderer,&r);

      r.x+=2;
      r.h=thumbHeightV;
      r.w=6;
      r.y=scrollOffsetV+1;
      SDL_SetRenderDrawColor(display->renderer,colors[0].r,colors[0].g,colors[0].b,255);
      SDL_RenderFillRect(display->renderer,&r);
    }

  }

  //reset viewport to full window
  SDL_RenderSetViewport(display->renderer,&origVP);
  font->fontSize=f;

}

void CDropDown::setDisplay(CDisplay* d){
  display=d;
  button.setDisplay(d);
  colors[0]=display->pal.buttons[0];
  colors[1]=display->pal.buttons[1];
  txtColor=display->pal.txtCheckbox;
}

void CDropDown::setFocus(CActiveFocus* f){
  activeFocus=f;
  button.setFocus(f);
}

void CDropDown::setFont(CFont* f){
  font=f;
  button.setFont(f);
  button.setCaption("v");
}

void CDropDown::setFontSize(int sz){
  if(sz<6)   sz=6;
  if(sz>20)  sz=20;
  fontSize = sz;
  szY = sz+4;
}

size_t CDropDown::size(){
  return items.size();
}

