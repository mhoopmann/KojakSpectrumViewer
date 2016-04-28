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

#ifndef _CCHECKBOX_H
#define _CCHECKBOX_H

#include "CActiveFocus.h"
#include "CDisplay.h"
#include "CFont.h"
#include <vector>

using namespace std;

class CCheckbox{
public:
  CCheckbox();
  ~CCheckbox();

  bool active;
  bool checked;

  int posX;
  int posY;
  int sz;
  string caption;
  
  int  getCaptionWidth();
  bool logic(int mouseX, int mouseY, int mouseButton);
  void render();
  void setDisplay(CDisplay* d);
  void setFocus(CActiveFocus* f);
  void setFont(CFont* f);

private:

  CActiveFocus* activeFocus;
  CDisplay* display;
  CFont* font;

  kvColor color;
  int txtColor;
  

};

#endif