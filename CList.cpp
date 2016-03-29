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
  items = new vector<sListItem>;
}

CList::~CList(){
  display = NULL;
  font = NULL;
  delete items;
}

sListItem& CList::operator[](const int& index){
  return items->at(index);
}

bool CList::addItem(char* str, int value){
  sListItem s;
  s.item=str;
  s.value=value;
  items->push_back(s);
  fixLayout();
  return true;
}

void CList::clear(){
  items->clear();
  selected=-1;
  showScrollbarV=false;
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
  contentSize = items->size()*szHeight; //size of entire text content
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
    return 2;
  } 

  scrollLockV=false;

  return 0;

}

void CList::render(){
  SDL_Rect r;
  SDL_Rect origVP;
  SDL_Rect vp;
  size_t i;
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
  SDL_SetRenderDrawColor(display->renderer,0,0,0,128);
  SDL_RenderFillRect(display->renderer,&r);

  //Draw list items
  font->fontSize=szFont;
  r.h=szHeight;
  r.w=szX;
  r.x=0;
  for(i=offset;i<items->size();i++){
    r.y=i*szHeight-(int)(scrollOffsetV*scrollJumpV);
    if(i==selected){
      SDL_SetRenderDrawColor(display->renderer,32,128,128,255);
      SDL_RenderFillRect(display->renderer,&r);
    }
    font->render(r.x+2,r.y+1,items->at(i).item);
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
}

void CList::setFont(CFont* f){
  font=f;
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
