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

#ifndef _CTOOLBAR_H
#define _CTOOLBAR_H

#include "CActiveFocus.h"
#include "CButton.h"
#include "CDisplay.h"
#include "CEditBox.h"
#include "CFont.h"
#include "CGfxCollection.h"
#include "CInput.h"
#include <vector>

using namespace std;

class CToolbar {
public:
  CToolbar();
  ~CToolbar();
  
  int   posX;
  int   posY;
  int   szX;
  int   szY;

  void addSeparator (int posX, int posY);
  void init         ();
  int  logic        (int mouseX, int mouseY, int mouseButton);
  int  processInput ();
  void render       ();
  void setDisplay   (CDisplay* d);
  void setFocus     (CActiveFocus* f);
  void setFont      (CFont* f);
  void setGfx       (CGfxCollection* g);
  void setInput     (CInput* i);  

  int scanNumber;

private:

  CDisplay* display;
  CFont*    font;
  CInput*   input;

  CActiveFocus* activeFocus;

  CButton   openButton;
  CButton   prevButton;
  CButton   nextButton;
  CButton   sfButton;
  CButton   aboutButton;
  CButton   setButton;
  CButton   pngButton;
  //CEditBox  jumpBox;

  vector<SDL_Rect> separators;

  bool    lockButton;

  kvColor colors[2];

};

#endif