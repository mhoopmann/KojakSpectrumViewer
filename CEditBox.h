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

#ifndef _CEDITBOX_H
#define _CEDITBOX_H

#include "CActiveFocus.h"
#include "CDisplay.h"
#include "CFont.h"
#include "CInput.h"
#include <string>

using namespace std;

class CEditBox {
public:
  CEditBox();
  ~CEditBox();
  
  int   posX;
  int   posY;
  int   szX;

  bool  active;
  bool  decimal;
  bool  negative;
  bool  numeric;

  void getCaption   (int* i, double* d, string* s);
  void clear        ();
  bool logic        (int mouseX, int mouseY, int mouseButton);
  int  processInput ();
  void render       ();
  void setCaption   (const char* str);
  void setCaption   (int i);
  void setCaption   (double d);
  void setCaption   (string s);
  void setDisplay   (CDisplay* d);
  void setFocus     (CActiveFocus* f);
  void setFont      (CFont* f);
  void setFontSize  (int sz);
  void setInput     (CInput* i);

private:

  CDisplay* display;
  CFont*    font;
  CInput*   input;

  CActiveFocus* activeFocus;

  string  content;
  int     cursor;
  int     fontSize;
  bool    lockButton;
  int     szY;
  Uint32  timer;

  kvColor colors[2];
  int     txtColor;

};

#endif