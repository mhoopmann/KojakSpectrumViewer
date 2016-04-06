#ifndef _CINPUT_H
#define _CINPUT_H

#ifdef GCC
#include <SDL2/SDL.h>
#else
#include <SDL.h>
#endif

#include <stdio.h>

enum {
  KEY_0,
  KEY_1,
  KEY_2,
  KEY_3,
  KEY_4,
  KEY_5,
  KEY_6,
  KEY_7,
  KEY_8,
  KEY_9,
  KEY_A,
  KEY_B,
  KEY_Q,
  KEY_UP,
  KEY_DOWN,
  KEY_LEFT,
  KEY_RIGHT,
  KEY_ENTER,
  KEY_BACKSPACE,
  KEY_DELETE
};

class CInput{
public:
  CInput();
  bool getButtonState (int k);
  bool getKeyState    (int k);
  int  mouseAction    ();
  bool isPressed      (int k);
  bool isReleased     (int k);
  void processEvent   (SDL_Event& e);
private:
  SDL_Event e;
  bool keyState[128];
  bool buttonState[3];
  int  lastButton;

  void setButton(Uint8 k, bool b);
  void setKey(SDL_Keycode k, bool b);
};

#endif
