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

#include "CTextBox.h"

CTextBox::CTextBox(){
  activeFocus = NULL;
  display = NULL;
  font = NULL;
  
  posX=0;
  posY=0;
  szX=0;
  szY=0;
  setSize(10);

  showScrollbarV=false;
  scrollLockV=false;
  scrollOffsetV=0;

  showScrollbarH=false;
  scrollLockH=false;
  scrollOffsetH=0;

  maxWidth=0;
  noScroll=false;

}

CTextBox::~CTextBox(){
  activeFocus = NULL;
  display = NULL;
  font = NULL;
}

void CTextBox::addText(char* str){
  string s=str;
  addText(s);
}

void CTextBox::addText(string& str){
  kvTextLine t;
  size_t i;
  int fontSize=font->fontSize;

  t.text=str;
  font->fontSize=szFont;
  t.width=font->getStringWidth(str);
  font->fontSize=fontSize;
  if(t.width>maxWidth) maxWidth=t.width;

  buffer.push_back(t);
  if(buffer.size()>100) {
    if(buffer[0].width==maxWidth){
      maxWidth=0;
      for(i=1;i<buffer.size();i++){
        if(buffer[i].width>maxWidth) maxWidth=buffer[i].width;
      }
    }
    buffer.pop_front();
  }

  fixLayout();
  
}

void CTextBox::clear(){
  buffer.clear();
  fixLayout();
}

void CTextBox::fixLayout(){
  if(noScroll) return;

  int contentSize;
  int scrollTrackSpace;
  int scrollThumbSpace;
  int viewSize;
  
  //Vertical Scrolling
  viewSize = szY-10;
  contentSize = (int)buffer.size()*szHeight+2; //size of entire text content
  if(viewSize>0 && contentSize>viewSize) {
    showScrollbarV=true;
    
    thumbHeightV = viewSize*viewSize/contentSize;
    thumbMaxV = viewSize-thumbHeightV;

    scrollTrackSpace = contentSize-viewSize;
    scrollThumbSpace = viewSize-thumbHeightV;
    scrollJumpV = (double)scrollTrackSpace/scrollThumbSpace;
    scrollOffsetV = 0;
  } else showScrollbarV=false;

  //Horizontal Scrolling
  viewSize = szX-10;
  contentSize = maxWidth+2; //size of maximum text line
  if(viewSize>0 && contentSize>viewSize) {
    showScrollbarH=true;
    
    thumbHeightH = viewSize*viewSize/contentSize;
    thumbMaxH = viewSize-thumbHeightH;

    scrollTrackSpace = contentSize-viewSize;
    scrollThumbSpace = viewSize-thumbHeightH;
    scrollJumpH = (double)scrollTrackSpace/scrollThumbSpace;
    scrollOffsetH = 0;
  } else showScrollbarH=false;
 
}

int CTextBox::logic(int mouseX, int mouseY, int mouseButton, bool mouseButton1){

  //this object is in a viewport. Check logic relative to the viewport
  //mouseX-=posX;
  //mouseY-=posY;

  if(mouseButton1 && showScrollbarV){
    //grabbed scrollbar
    if(mouseX>=szX-8 && mouseX<=szX-2 && mouseY>=posY+scrollOffsetV && mouseY<=posY+scrollOffsetV+thumbHeightV){
      if(scrollLockV){
        //printf("Scrolled: %d %d\n",mouseY, lastMouseY);
        scrollOffsetV+=(mouseY-lastMouseY);
        if(scrollOffsetV>thumbMaxV) scrollOffsetV=thumbMaxV;
        if(scrollOffsetV<0) scrollOffsetV=0;
        lastMouseY=mouseY;
      } else {
        scrollLockV=true;
        lastMouseY=mouseY;
        //printf("Locking %d\n",lastMouseY);
      }
      return 1;
    } else if(scrollLockV){
      //printf("Scrolled: %d %d\n",mouseY, lastMouseY);
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

  if(mouseButton1 && showScrollbarH){
    //grabbed scrollbar
    if(mouseX>=scrollOffsetH && mouseX<=scrollOffsetH+thumbHeightH && mouseY>=posY+szY-8 && mouseY<=posY+szY-2){
      if(scrollLockH){
        //printf("Scrolled: %d %d\n",mouseY, lastMouseY);
        scrollOffsetH+=(mouseX-lastMouseX);
        if(scrollOffsetH>thumbMaxH) scrollOffsetH=thumbMaxH;
        if(scrollOffsetH<0) scrollOffsetH=0;
        lastMouseX=mouseX;
      } else {
        scrollLockH=true;
        lastMouseX=mouseX;
        //printf("Locking %d\n",lastMouseY);
      }
      return 1;
    } else if(scrollLockH){
      //printf("Scrolled: %d %d\n",mouseY, lastMouseY);
      scrollOffsetH+=(mouseX-lastMouseX);
      if(scrollOffsetH>thumbMaxH) scrollOffsetH=thumbMaxH;
      if(scrollOffsetH<0) scrollOffsetH=0;
      lastMouseX=mouseX;
      return 1;
    }
  } 

  if(mouseButton==1 && scrollLockH){
    scrollLockH=false;
    return 1;
  }

  scrollLockH=false;
  scrollLockV=false;
  return 0; 
}

void CTextBox::render(){
  SDL_Rect origVP;
  SDL_Rect vp;
  SDL_Rect r;
  int x,y;
  size_t i;
  int fontSize=font->fontSize;

  SDL_RenderGetViewport(display->renderer,&origVP);

  //Reset the viewport to just this text box;
  vp.x=posX;
  vp.y=posY;
  vp.w=szX;
  vp.h=szY;
  SDL_RenderSetViewport(display->renderer,&vp);

  r=vp;
  r.x=0;
  r.y=0;
  SDL_SetRenderDrawColor(display->renderer,colors[0].r,colors[0].g,colors[0].b,255);
  SDL_RenderFillRect(display->renderer,&r);

  //New viewport for scrollable content
  //also excludes area that scroll bars will occupy
  if(!noScroll){
    vp.w -= 10;
    vp.h -= 10;
    SDL_RenderSetViewport(display->renderer,&vp);
  }

  if(!noScroll){
    //set the scrollbar offsets
    font->fontSize=szFont;
    x=2-(int)(scrollOffsetH*scrollJumpH);
    y=2-(int)(scrollOffsetV*scrollJumpV);
    for(i=0;i<buffer.size();i++){
      if(y>-12) font->render(x,y,&buffer[i].text[0],txtColor);
      y+=szHeight;
      if(y>szY) break;
    }
  } else {
    //vertically center what can be seen
    i=(szY-2)/szHeight;
    x=2;
    y=(szY-(int)i*szHeight)/2;
    for(i=0; i<buffer.size(); i++){
      font->render(x, y, &buffer[i].text[0], txtColor);
      y+=szHeight;
      if(y>szY) break;
    }
  }

  //expand viewport for scrollbars
  if(!noScroll){
    vp.w+=10;
    vp.h+=10;
    SDL_RenderSetViewport(display->renderer,&vp);
  }

  //draw the scrollbar
  if(showScrollbarV){
    r.x=vp.w-8;
    r.h=thumbHeightV;
    r.w=6;
    r.y=scrollOffsetV;
    SDL_SetRenderDrawColor(display->renderer,185,185,185,255);
    SDL_RenderFillRect(display->renderer,&r);
  }

  //draw the scrollbar
  if(showScrollbarH){
    r.x=scrollOffsetH;
    r.h=6;
    r.w=thumbHeightH;
    r.y=vp.h-8;
    SDL_SetRenderDrawColor(display->renderer,185,185,185,255);
    SDL_RenderFillRect(display->renderer,&r);
  }

  //reset viewport to full window
  SDL_RenderSetViewport(display->renderer,&origVP);

  font->fontSize=fontSize;
  
}

void CTextBox::setDisplay(CDisplay* d){
  display = d;
  colors[0]=d->pal.textBox[0];
  colors[1]=d->pal.textBox[1];
  txtColor=d->pal.txtTextBox;
}

void CTextBox::setFocus(CActiveFocus* f){
  activeFocus = f;
}

void CTextBox::setFont(CFont* f){
  font = f;
}

void CTextBox::setSize(int sz){
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
