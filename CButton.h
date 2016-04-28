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

#ifndef _CBUTTON_H
#define _CBUTTON_H

#include "CActiveFocus.h"
#include "CDisplay.h"
#include "CGfxCollection.h"
#include "CGraphic.h"
#include "CFont.h"
#include <string>

using namespace std;

class CButton{
public:
  CButton();
  ~CButton();
  
  bool  active;
  int   posX;
  int   posY;
  int   szFont;
  int   szX;
  int   szY;

  bool logic        (int mouseX, int mouseY, int mouseButton);
  void render       ();
  void setCaption   (char* str);
  void setCaption   (string str);
  void setDisplay   (CDisplay* d);
  void setFocus     (CActiveFocus* f);
  void setFont      (CFont* f);
  void setGfx       (CGfxCollection* g);
  void setImage     (int index);
  

private:

  CDisplay* display;
  CFont*    font;
  CGraphic* image;  //Image or text for button

  CActiveFocus* activeFocus;

  string  caption;
  bool    lockButton;
  bool    highlight;
  int     imgIndex;
  int     textWidth;
  int     textHeight;

  kvColor colors[5];
  int txtColors[2];

};

#endif