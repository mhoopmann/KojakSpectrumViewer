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

#include "CList.h"

CList::CList(){
  offset=0;
  posX=0;
  posY=0;
  selected=-1;
  szFont=16;
  szHeight=20;
  szX=0;
  szY=0;
  display = NULL;
  font = NULL;
  icons = NULL;
  items = new vector<sListItem>;
}

CList::~CList(){
  display = NULL;
  font = NULL;
  icons = NULL;
  delete items;
}

sListItem& CList::operator[](const int& index){
  return items->at(index);
}

bool CList::addItem(char* str, int value, int icon){
  sListItem s;
  s.item=str;
  s.icon=icon;
  s.value=value;
  items->push_back(s);
  fixLayout();
  return true;
}

void CList::clear(){
  items->clear();
  selected=-1;
  showScrollbarV=false;
  fixLayout();
}

void CList::clearSelected(){
  selected=-1;
}

void CList::deleteItem(int index){
  items->erase(items->begin()+index);
  if(selected==index) selected=-1;
  fixLayout();
}

void CList::fixLayout(){
  int contentSize;
  int scrollTrackSpace;
  int scrollThumbSpace;
  int viewSize;
  
  //Vertical Scrolling
  viewSize = szY;
  contentSize = (int)items->size()*szHeight; //size of entire text content
  if(viewSize>0 && contentSize>viewSize) {
    showScrollbarV=true;
    
    thumbHeightV = viewSize*viewSize/contentSize;
    if(thumbHeightV<8) thumbHeightV=8;
    thumbMaxV = viewSize-thumbHeightV;

    scrollTrackSpace = contentSize-viewSize;
    scrollThumbSpace = viewSize-thumbHeightV;
    scrollJumpV = (double)scrollTrackSpace/scrollThumbSpace;
    scrollOffsetV = 0;
  } else {
    scrollOffsetV = 0;
    scrollJumpV = 0;
    showScrollbarV=false;
  }
 
}

int CList::getSelected(){
  return selected;
}

sListItem CList::getSelectedItem(){
  if(selected>-1) return items->at(selected);
  
  sListItem s;
  s.item.clear();
  s.value=-1;
  return s;
}

int CList::logic(int mouseX, int mouseY, int mouseButton, bool mouseButton1){
  int i;

  if(mouseButton1 && showScrollbarV){
    //grabbed scrollbar
    if(mouseX>=posX+szX-8 && mouseX<=posX+szX-2 && mouseY>=posY+scrollOffsetV && mouseY<=posY+scrollOffsetV+thumbHeightV){
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

  if(mouseButton==1 && scrollLockV){
    scrollLockV=false;
    return 1;
  }

  if(mouseButton==1 && mouseX>=posX && mouseX<=(posX+szX) && mouseY>=posY && mouseY<=posY+szY){
    i=mouseY-posY+(int)(scrollOffsetV*scrollJumpV);
    i/=szHeight;
    if(i<(int)items->size()) selected = i;
    else selected = -1;
    return 2;
  } 

  //Check for highlight
  if(mouseX>=posX && mouseX<=(posX+szX) && mouseY>=posY && mouseY<=posY+szY){
    int i=mouseY-posY+(int)(scrollOffsetV*scrollJumpV);
    i/=szHeight;
    highlight=-1;
    if(i<(int)items->size()) highlight = i;
  } else {
    highlight=-1;
  }

  scrollLockV=false;

  return 0;

}

void CList::render(){
  SDL_Rect r;
  SDL_Rect origVP;
  SDL_Rect vp;
  size_t i;
  int w,h;
  int fontSize=font->fontSize;

  SDL_RenderGetViewport(display->renderer,&origVP);

  //Set the viewport
  vp.h=szY;
  vp.w=szX;
  vp.x=posX;
  vp.y=posY;
  SDL_RenderSetViewport(display->renderer,&vp);

  //Draw background
  r.h=szY;
  r.w=szX;
  r.x=0;
  r.y=0;
  SDL_SetRenderDrawColor(display->renderer,colors[0].r,colors[0].g,colors[0].b,255);
  SDL_RenderFillRect(display->renderer,&r);

  //Draw list items
  font->fontSize=szFont;
  r.h=szHeight;
  r.w=szX;
  r.x=0;
  for(i=offset;i<items->size();i++){
    r.y=(int)i*szHeight-(int)(scrollOffsetV*scrollJumpV);
    if(i==selected){
      SDL_SetRenderDrawColor(display->renderer,colors[2].r,colors[2].g,colors[2].b,255);
      SDL_RenderFillRect(display->renderer,&r);
    }
    if(highlight==(int)i){
      SDL_SetRenderDrawColor(display->renderer, colors[1].r, colors[1].g, colors[1].b, 255);
      SDL_RenderFillRect(display->renderer, &r);
    }
    if(items->at(i).icon>-1){
      switch(szFont){
      case 8: w=6; h=6; break;
      case 10: w=8; h=8; break;
      case 12: w=10; h=10; break;
      case 14: w=12; h=12; break;
      case 16: w=16; h=16; break;
      default: w=0; h=0; break;
      }
      icons->render(display->renderer,items->at(i).icon,r.x+2,r.y+1,w,h);
      font->render(r.x+w+4,r.y+1,items->at(i).item,txtColor);
    } else {
      font->render(r.x+2, r.y+1, items->at(i).item, txtColor);
    }
  }

  //Draw scrollbar
  if(showScrollbarV){
    r.x=szX-10;
    r.y=0;
    r.w=10;
    r.h=szY;
    SDL_SetRenderDrawColor(display->renderer,255,255,255,255);
    SDL_RenderFillRect(display->renderer,&r);

    r.x+=2;
    r.h=thumbHeightV;
    r.w=6;
    r.y=scrollOffsetV;
    SDL_SetRenderDrawColor(display->renderer,185,185,185,255);
    SDL_RenderFillRect(display->renderer,&r);
  }

  SDL_RenderSetViewport(display->renderer,&origVP);
  font->fontSize=fontSize;

}

void CList::setDisplay(CDisplay* d){
  display=d;
  colors[0]=display->pal.list[0];
  colors[1]=display->pal.list[1];
  colors[2]=display->pal.list[2];
  txtColor=display->pal.txtList;
}

void CList::setFont(CFont* f){
  font=f;
}

void CList::setGfx(CGfxCollection* g){
  icons=g->icons;
}

bool CList::setSelected(int index){
  if(index<0 || index>=(int)items->size()){
    selected=-1;
    return false;
  }
  selected=index;
  return true;
}

void CList::setSize(int sz){
  szFont=sz;
  switch(szFont){
    case 6: szHeight=8; break;
    case 8: szHeight=10; break;
    case 10: szHeight=12; break;
    case 12: szHeight=14; break;
    case 14: szHeight=16; break;
    case 16: szHeight=20; break;
    default: szHeight=18; break;
  }
}

size_t CList::size(){
  return items->size();
}
