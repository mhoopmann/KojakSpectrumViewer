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
  negative = false;
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
  decimal=false;
  negative=false;
}

void CEditBox::getCaption(int* i, double* d, string* s){
  if(i!=NULL) *i=atoi(&content[0]);
  if(d!=NULL) *d=atof(&content[0]);
  if(s!=NULL) *s=content;
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
    } else if(numeric && c==45 && !negative){
      if(cursor==0) {
        negative=true;
        content.insert(content.begin()+cursor, c);
        cursor++;
      }
    } else if(numeric && c==46 && !decimal){
      decimal=true;
      content.insert(content.begin()+cursor, c);
      cursor++;
    } else if (c == 127) {
      if (cursor<(int)content.size()) {
        if(numeric && decimal && content[cursor]=='.') decimal=false;
        if(numeric && negative && content[cursor]=='-') negative=false;
        content.erase(cursor, 1);
      }
    } else if (c == 8) {
      if (cursor>0){
        if(numeric && decimal && content[cursor-1]=='.') decimal=false;
        if(numeric && negative && content[cursor-1]=='-') negative=false;
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
  if(this==activeFocus->focus) SDL_SetRenderDrawColor(display->renderer,colors[1].r,colors[1].g,colors[1].b,255);
  else SDL_SetRenderDrawColor(display->renderer, colors[0].r, colors[0].g, colors[0].b, 255);
  SDL_RenderFillRect(display->renderer,&r);

  //Draw Text
  font->fontSize = fontSize;
  font->render(posX+4,posY+3,content,txtColor);

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

void CEditBox::setCaption(const char* str){
  content=str;
  cursor=(int)content.size();
}

void CEditBox::setCaption(int i){
  char str[256];
  sprintf(str, "%d", i);
  content=str;
  cursor=(int)content.size();
}

void CEditBox::setCaption(double d){
  char str[256];
  sprintf(str, "%g", d);
  content=str;
  cursor=(int)content.size();
}

void CEditBox::setCaption(string s){
  content=s;
  cursor=(int)content.size();
}

void CEditBox::setDisplay(CDisplay* d){
  display=d;
  colors[0]=d->pal.textBox[0];
  colors[1]=d->pal.textBox[1];
  txtColor=d->pal.txtTextBox;
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
  szY = sz+4;
}

void CEditBox::setInput(CInput* i){
  input=i;
}
