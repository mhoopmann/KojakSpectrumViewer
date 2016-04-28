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

#ifndef _CINPUT_H
#define _CINPUT_H

#ifdef GCC
#include <SDL2/SDL.h>
#else
#include <SDL.h>
#endif

#include <deque>
#include <stdio.h>

using namespace std;

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
  KEY_CAPS,
  KEY_UP,
  KEY_DOWN,
  KEY_LEFT,
  KEY_RIGHT,
  KEY_ENTER,
  KEY_LSHIFT,
  KEY_RSHIFT,
  KEY_BACKSPACE,
  KEY_DELETE
};

class CInput{
public:
  CInput();

  void clear          ();

  char getBuffer      (bool key=true); //false returns cursor buffer
  bool getButtonState (int k);
  bool getKeyState    (int k);
  int  mouseAction    ();
  bool isPressed      (int k);
  bool isReleased     (int k);
  void processEvent   (SDL_Event& e);

  size_t size         (bool key=true); //false returns cursor buffer

private:
  SDL_Event e;
  bool keyState[128];
  bool buttonState[3];
  bool keyShift;
  int  lastButton;

  deque<char> keyBuffer;
  deque<char> cursorBuffer;

  void add(char c, bool key=true);  //false adds to cursor buffer
  void setButton(Uint8 k, bool b);
  void setKey(SDL_Keycode k, bool b);
};

#endif
