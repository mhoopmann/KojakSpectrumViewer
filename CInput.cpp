#include "CInput.h"

CInput::CInput(){
  int i;
  for(i=0;i<128;i++) keyState[i]=false;
  for(i=0;i<3;i++) buttonState[i]=false;
  lastButton=0;
}

bool CInput::getButtonState(int k){
  return buttonState[k];
}

bool CInput::getKeyState(int k){
  return keyState[k];
}

bool CInput::isPressed(int k){
  if(keyState[k]) return true;
  else return false;
}

bool CInput::isReleased(int k){
  if(keyState[k]) return false;
  else return true;
}

int CInput::mouseAction(){
  //printf("mouseAction %d\n",lastButton);
  int i=lastButton;
  if(lastButton>0) lastButton=0;
  return i;
}

void CInput::processEvent(SDL_Event& e){
  if( e.type == SDL_KEYDOWN ) setKey(e.key.keysym.sym,true);
  else if( e.type == SDL_KEYUP ) setKey(e.key.keysym.sym,false);
  else if( e.type == SDL_MOUSEBUTTONDOWN ) setButton(e.button.button,true);
  else if( e.type == SDL_MOUSEBUTTONUP ) setButton(e.button.button,false);
}

void CInput::setButton(Uint8 k, bool b){
  switch(k){
    case SDL_BUTTON_LEFT:   
      buttonState[0]=b; 
      if(!b) {
        printf("Button 1 release\n");
        lastButton=1; 
      }
      break;
    case SDL_BUTTON_RIGHT:  
      buttonState[1]=b; 
      if(!b) {
        printf("Button 2 release\n");
        lastButton=2; 
      }
      break;
    case SDL_BUTTON_MIDDLE: 
      buttonState[2]=b; 
      if(!b) {
        printf("Button 3 release\n");
        lastButton=3; 
      }
      break;
    default: 
      break;
  }
}

void CInput::setKey(SDL_Keycode k, bool b){
  switch(k){
    case SDLK_0:      keyState[KEY_0]=b; break;
    case SDLK_1:      keyState[KEY_1]=b; break;
    case SDLK_2:      keyState[KEY_2]=b; break;
    case SDLK_3:      keyState[KEY_3]=b; break;
    case SDLK_4:      keyState[KEY_4]=b; break;
    case SDLK_5:      keyState[KEY_5]=b; break;
    case SDLK_6:      keyState[KEY_6]=b; break;
    case SDLK_7:      keyState[KEY_7]=b; break;
    case SDLK_8:      keyState[KEY_8]=b; break;
    case SDLK_9:      keyState[KEY_9]=b; break;
    case SDLK_q:      keyState[KEY_Q]=b; break;
    case SDLK_UP:     keyState[KEY_UP]=b; break;
    case SDLK_DOWN:   keyState[KEY_DOWN]=b; break;
    case SDLK_LEFT:   keyState[KEY_LEFT]=b; break;
    case SDLK_RIGHT:  keyState[KEY_RIGHT]=b; break;
    case SDLK_RETURN: keyState[KEY_ENTER]=b; break;
    case SDLK_BACKSPACE: keyState[KEY_BACKSPACE]=b; break;
    case SDLK_DELETE: keyState[KEY_DELETE]=b; break;
    default: break;
  }
}
