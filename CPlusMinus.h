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

#ifndef _CPLUSMINUS_H
#define _CPLUSMINUS_H

#include "CActiveFocus.h"
#include "CButton.h"
#include "CDisplay.h"
#include "CFont.h"
#include <string>
#include <vector>

using namespace std;

class CPlusMinus{
public:
  CPlusMinus();
  ~CPlusMinus();

  bool active;

  int posX;
  int posY;
  int szX;

  int max;
  int min;

  int value;

  int     logic(int mouseX, int mouseY, int mouseButton, bool mouseButton1);
  void    render();
  void    setDisplay(CDisplay* d);
  void    setFocus(CActiveFocus* f);
  void    setFont(CFont* f);
  void    setFontSize(int sz);
  size_t  size();

private:

  CActiveFocus* activeFocus;
  CDisplay* display;
  CFont* font;

  CButton buttonMinus;
  CButton buttonPlus;

  int     fontSize;
  int     szY;

  kvColor colors[2];
  int     txtColor;

};

#endif