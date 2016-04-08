#include "CEditBox.h"

CEditBox::CEditBox(){
  activeFocus = NULL;
  display = NULL;
  font    = NULL;
  input   = NULL;

  lockButton=false;
  cursor = 0;
  posX = 0;
  posY = 0;
  szX  = 0;
  szY  = 0;
  numeric = false;
  decimal = false;
  active = true;

  content.clear();
  timer = SDL_GetTicks();
}

CEditBox::~CEditBox(){
  activeFocus = NULL;
  display = NULL;
  font    = NULL;
  input   = NULL;
}

void CEditBox::clear(){
  content.clear();
  cursor=0;
}

bool CEditBox::logic(int mouseX, int mouseY, int mouseButton){
  if(!active) return false;

  if(mouseX>=posX && mouseX<=(posX+szX) && mouseY>=posY && mouseY<=(posY+szY)) {
    //highlight=true;
    if(mouseButton==1 && !lockButton){
      input->clear();  //clear anything still in buffer
      activeFocus->focus=this;
      lockButton=true;

      int fs=font->fontSize;
      int x=mouseX-posX-4;
      cursor=0;
      font->fontSize=fontSize;
      if(content.size()>0) {
        cursor=0;
        while(font->getStringWidth(content.substr(0, cursor))<x) {
          cursor++;
          if(cursor>(int)content.size()) break;
        }
        cursor--;
        if(cursor<0) cursor=0;
      }
      font->fontSize=fs;

      return true;
    }
  } else {
    //highlight=false;
  }
  if(lockButton && mouseButton==0) lockButton=false;

  return false;  
}

int CEditBox::processInput(){
  char c;
  while (input->size() > 0){
    c = input->getBuffer();
    if (!numeric && c > 31 && c < 127) {
      content.insert(content.begin()+cursor, c);
      cursor++;
    } else if(numeric && c>47 && c<58) {
      content.insert(content.begin()+cursor, c);
      cursor++;
    } else if(numeric && c==46 && !decimal){
      decimal=true;
      content.insert(content.begin()+cursor, c);
      cursor++;
    } else if (c == 127) {
      if (cursor<(int)content.size()) {
        if(numeric && decimal && content[cursor]=='.') decimal=false;
        content.erase(cursor, 1);
      }
    } else if (c == 8) {
      if (cursor>0){
        if(numeric && decimal && content[cursor-1]=='.') decimal=false;
        content.erase(cursor - 1, 1);
        cursor--;
      }
    } else if (c == 13 && numeric){
      int i;
      i = atoi(&content[0]);
      return i; //only exporting integers at the moment
    }
  }

  while(input->size(false)>0){
    c = input->getBuffer(false);
    if(c==1){
      if(cursor>0) cursor--;
    } else if(c==2){
      if(cursor<(int)content.size()) cursor++;
    }
  }

  return -1;
}
  
void CEditBox::render(){
  SDL_Rect r;
  int fs = font->fontSize;

  //Draw background
  r.w=szX;
  r.h=szY;
  r.x=posX;
  r.y=posY;
  if(this==activeFocus->focus) SDL_SetRenderDrawColor(display->renderer,0,0,64,255);
  else SDL_SetRenderDrawColor(display->renderer,64,64,64,255);
  SDL_RenderFillRect(display->renderer,&r);

  //Draw Text
  font->fontSize = fontSize;
  font->render(posX+4,posY+3,content);

  //Draw Cursor
  if(this==activeFocus->focus){
    if(SDL_GetTicks()-timer>500){
      SDL_SetRenderDrawColor(display->renderer,255,255,255,255);
      r.x=font->getStringWidth(content.substr(0,cursor))+posX+4;
      r.y=posY+2;
      SDL_RenderDrawLine(display->renderer,r.x,r.y,r.x,r.y+szY-5);
    }
    if(SDL_GetTicks()-timer>1000){
      timer=SDL_GetTicks();
    }
  }

  font->fontSize=fs;

}

void CEditBox::setDisplay(CDisplay* d){
  display=d;
}

void CEditBox::setFocus(CActiveFocus* f){
  activeFocus=f;
}

void CEditBox::setFont(CFont* f){
  font=f;
}

void CEditBox::setFontSize(int sz){
  if(sz<6)   sz=6;
  if(sz>20)  sz=20;
  fontSize = sz;
  szY = sz+7;
}

void CEditBox::setInput(CInput* i){
  input=i;
}
