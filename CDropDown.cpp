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
  selected = 0;
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
  selected=0;
  highlight=-1;
}

void CDropDown::fixLayout(){
  int contentSize;
  int scrollTrackSpace;
  int scrollThumbSpace;
  int viewSize;
  
  //Vertical Scrolling
  viewSize = szDrop-2;
  contentSize = items.size()*15; //size of entire text content
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
  return items[selected];
}

int CDropDown::logic(int mouseX, int mouseY, int mouseButton, bool mouseButton1){

  if(mouseButton1 && dropped && showScrollbarV){
    //printf("%d %d :: %d %d\n",mouseX,mouseY,szX-8,posY+szY+scrollOffsetV+1);
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

  if(mouseButton==1 && scrollLockV){
    scrollLockV=false;
    return 1;
  }

  if(mouseButton==1 && dropped && mouseX>=posX && mouseX<=(posX+szX-10) && mouseY>=posY+szY+1 && mouseY<=posY+szY+szDrop-1){
    int i=mouseY-posY-15+(int)(scrollOffsetV*scrollJumpV);
    i/=15;
    if(i<(int)items.size()) selected = i;
    dropped=false;
    return 2;
  } 

  if(button.logic(mouseX,mouseY,mouseButton)){
    if(dropped) dropped=false;
    else dropped=true;
  }

  if(dropped && mouseX>=posX && mouseX<=(posX+szX-10) && mouseY>=posY+szY+1 && mouseY<=posY+szY+szDrop-1){
    int i=mouseY-posY-15+(int)(scrollOffsetV*scrollJumpV);
    i/=15;
    if(i<(int)items.size()) highlight = i;
  } else {
    highlight=-1;
  }

  scrollLockV=false;

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
  font->render(posX+4,posY+2,items[selected],1);
  
  //Draw list
  if(dropped){
    vp.h=szDrop;
    vp.w=szX;
    vp.x=posX;
    vp.y=posY+szY;
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
      if(r.y+15<0) continue;
      if(highlight==(int)i){
        r2.x=1;
        r2.y=r.y;
        r2.w=szX-2;
        r2.h=15;
        SDL_SetRenderDrawColor(display->renderer,colors[1].r,colors[1].g,colors[1].b,255);
        SDL_RenderFillRect(display->renderer,&r2);
      }
      font->render(r.x,r.y,items[i],1);
      r.y+=15;
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
