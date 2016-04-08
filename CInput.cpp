#include "CInput.h"

CInput::CInput(){
  int i;
  for(i=0;i<128;i++) keyState[i]=false;
  for(i=0;i<3;i++) buttonState[i]=false;
  lastButton=0;
  keyShift = false;
}

void CInput::add(char c, bool key){
  if(key){
    keyBuffer.push_back(c);
    if (keyBuffer.size() == 256) keyBuffer.pop_front();
  } else {
    cursorBuffer.push_back(c);
    if(cursorBuffer.size()==256) cursorBuffer.pop_front();
  }
}

void CInput::clear(){
  keyBuffer.clear();
  cursorBuffer.clear();
}

char CInput::getBuffer(bool key){
  if(key){
    if (keyBuffer.size() > 0){
      char c = keyBuffer[0];
      keyBuffer.pop_front();
      return c;
    }
    return 0;
  }

  if(cursorBuffer.size() > 0){
    char c = cursorBuffer[0];
    cursorBuffer.pop_front();
    return c;
  }
  return 0;
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
      if(!b) lastButton=1; 
      break;
    case SDL_BUTTON_RIGHT:  
      buttonState[1]=b; 
      if(!b) lastButton=2; 
      break;
    case SDL_BUTTON_MIDDLE: 
      buttonState[2]=b; 
      if(!b) lastButton=3; 
      break;
    default: 
      break;
  }
}

void CInput::setKey(SDL_Keycode k, bool b){

  //Set key states first
  switch (k){
  case SDLK_BACKSPACE: keyState[KEY_BACKSPACE] = b; break;
  case SDLK_RETURN: keyState[KEY_ENTER] = b; break;
  case SDLK_0:      keyState[KEY_0] = b; break;
  case SDLK_1:      keyState[KEY_1] = b; break;
  case SDLK_2:      keyState[KEY_2] = b; break;
  case SDLK_3:      keyState[KEY_3] = b; break;
  case SDLK_4:      keyState[KEY_4] = b; break;
  case SDLK_5:      keyState[KEY_5] = b; break;
  case SDLK_6:      keyState[KEY_6] = b; break;
  case SDLK_7:      keyState[KEY_7] = b; break;
  case SDLK_8:      keyState[KEY_8] = b; break;
  case SDLK_9:      keyState[KEY_9] = b; break;
  case SDLK_q:      keyState[KEY_Q] = b; break;
  case SDLK_DELETE: keyState[KEY_DELETE] = b; break;
  case SDLK_UP:     keyState[KEY_UP] = b; break;
  case SDLK_DOWN:   keyState[KEY_DOWN] = b; break;
  case SDLK_LEFT:   
    keyState[KEY_LEFT] = b; 
    if(b) add(1, false);
    break;
  case SDLK_RIGHT:  
    keyState[KEY_RIGHT] = b; 
    if(b) add(2, false);
    break;
  case SDLK_CAPSLOCK: 
    keyState[KEY_CAPS] = b; 
    if (keyState[KEY_CAPS]) keyShift = !keyShift; 
    break;
  case SDLK_LSHIFT: 
    keyState[KEY_LSHIFT] = b;
    if (keyState[KEY_LSHIFT] || keyState[KEY_RSHIFT]) keyShift = true;
    else keyShift = false;
    if (keyState[KEY_CAPS]) keyShift = !keyShift;
    break;
  case SDLK_RSHIFT: 
    keyState[KEY_RSHIFT] = b; 
    if (keyState[KEY_LSHIFT] || keyState[KEY_RSHIFT]) keyShift = true;
    else keyShift = false;
    if (keyState[KEY_CAPS]) keyShift = !keyShift;
    break;
  default: break;
  }

  //Buffer keypress if necessary
  if (b && k<128) {
    if (keyShift && k > 96 && k < 123) add((char)(k - 32));
    else add((char)k);
  }

}

size_t CInput::size(bool key){
  if(key) return keyBuffer.size();
  return cursorBuffer.size();
}

