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
  for(int i=0;i<10;i++) lockKey[i]=false;

  content.clear();
  timer = SDL_GetTicks();
}

CEditBox::~CEditBox(){
  activeFocus = NULL;
  display = NULL;
  font    = NULL;
  input   = NULL;
}

bool CEditBox::logic(int mouseX, int mouseY, int mouseButton){
 if(mouseX>=posX && mouseX<=(posX+szX) && mouseY>=posY && mouseY<=(posY+szY)) {
    //highlight=true;
    if(mouseButton==1 && !lockButton){
      activeFocus->focus=this;
      lockButton=true;

      int x=mouseX-posX-4;
      cursor=x/9;
      if(cursor<0) cursor=0;
      if(cursor>(int)content.size()) cursor=(int)content.size();

      return true;
    }
  } else {
    //highlight=false;
  }
  if(lockButton && mouseButton==0) lockButton=false;

  return false;  
}

int CEditBox::processInput(){
  if(!lockKey[0] && input->isPressed(KEY_0)) {
    content.insert(cursor,"0");
    cursor++;
    lockKey[0]=true;
  }
  if(!lockKey[1] && input->isPressed(KEY_1)) {
    content.insert(cursor,"1");
    cursor++;
    lockKey[1]=true;
  }
  if(!lockKey[2] && input->isPressed(KEY_2)) {
    content.insert(cursor,"2");
    cursor++;
    lockKey[2]=true;
  }
  if(!lockKey[3] && input->isPressed(KEY_3)) {
    content.insert(cursor,"3");
    cursor++;
    lockKey[3]=true;
  }
  if(!lockKey[4] && input->isPressed(KEY_4)) {
    content.insert(cursor,"4");
    cursor++;
    lockKey[4]=true;
  }
  if(!lockKey[5] && input->isPressed(KEY_5)) {
    content.insert(cursor,"5");
    cursor++;
    lockKey[5]=true;
  }
  if(!lockKey[6] && input->isPressed(KEY_6)) {
    content.insert(cursor,"6");
    cursor++;
    lockKey[6]=true;
  }
  if(!lockKey[7] && input->isPressed(KEY_7)) {
    content.insert(cursor,"7");
    cursor++;
    lockKey[7]=true;
  }
  if(!lockKey[8] && input->isPressed(KEY_8)) {
    content.insert(cursor,"8");
    cursor++;
    lockKey[8]=true;
  }
  if(!lockKey[9] && input->isPressed(KEY_9)) {
    content.insert(cursor,"9");
    cursor++;
    lockKey[9]=true;
  }
  if(!lockKey[10] && input->isPressed(KEY_BACKSPACE)){
    if(cursor>0){
      content.erase(cursor-1,1);
      cursor--;
    }
    lockKey[10]=true;
  }
  if(!lockKey[11] && input->isPressed(KEY_DELETE)){
    if(cursor<(int)content.size()) content.erase(cursor,1);
    lockKey[11]=true;
  }
  if(!lockKey[12] && input->isPressed(KEY_ENTER)){
    int i;
    i=atoi(&content[0]);
    return i;
  }
  if(lockKey[0] && input->isReleased(KEY_0)) lockKey[0]=false;
  if(lockKey[1] && input->isReleased(KEY_1)) lockKey[1]=false;
  if(lockKey[2] && input->isReleased(KEY_2)) lockKey[2]=false;
  if(lockKey[3] && input->isReleased(KEY_3)) lockKey[3]=false;
  if(lockKey[4] && input->isReleased(KEY_4)) lockKey[4]=false;
  if(lockKey[5] && input->isReleased(KEY_5)) lockKey[5]=false;
  if(lockKey[6] && input->isReleased(KEY_6)) lockKey[6]=false;
  if(lockKey[7] && input->isReleased(KEY_7)) lockKey[7]=false;
  if(lockKey[8] && input->isReleased(KEY_8)) lockKey[8]=false;
  if(lockKey[9] && input->isReleased(KEY_9)) lockKey[9]=false;
  if(lockKey[10] && input->isReleased(KEY_BACKSPACE)) lockKey[10]=false;
  if(lockKey[11] && input->isReleased(KEY_DELETE)) lockKey[11]=false;
  if(lockKey[12] && input->isReleased(KEY_ENTER)) lockKey[12]=false;
  return -1;
}
  
void CEditBox::render(){
  SDL_Rect r;

  //Draw background
  r.w=szX;
  r.h=szY;
  r.x=posX;
  r.y=posY;
  if(this==activeFocus->focus) SDL_SetRenderDrawColor(display->renderer,0,0,64,255);
  else SDL_SetRenderDrawColor(display->renderer,64,64,64,255);
  SDL_RenderFillRect(display->renderer,&r);

  //Draw Text
  font->render(posX+4,posY+4,content);

  //Draw Cursor
  if(this==activeFocus->focus){
    if(SDL_GetTicks()-timer>500){
      SDL_SetRenderDrawColor(display->renderer,255,255,255,255);
      r.x=cursor*9+posX+4;
      r.y=posY+4;
      SDL_RenderDrawLine(display->renderer,r.x,r.y,r.x,r.y+20);
    }
    if(SDL_GetTicks()-timer>1000){
      timer=SDL_GetTicks();
    }
  }

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

void CEditBox::setInput(CInput* i){
  input=i;
}
