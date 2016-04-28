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

#ifndef _CSLIDER_H
#define _CSLIDER_H

#include "CActiveFocus.h"
#include "CDisplay.h"

class CSlider{
public:
  int posX;
  int posY;
  int szX;
  int szY;
  bool vertical;

  CSlider();
  ~CSlider();

  bool  checkLocked();
  int   logic(int mouseX, int mouseY, int mouseButton, bool mouseButton1);
  bool  render();
  void  setDisplay(CDisplay* d);
  void  setFocus(CActiveFocus* f);

private:

  CActiveFocus* activeFocus;
  CDisplay* display;

  bool sliderLock;
  int lastMouse;

  kvColor color;

};

#endif