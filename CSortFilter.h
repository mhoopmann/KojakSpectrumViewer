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

#ifndef _CSORTFILTER_H
#define _CSORTFILTER_H

#include "CActiveFocus.h"
#include "CButton.h"
#include "CCheckbox.h"
#include "CDisplay.h"
#include "CDropDown.h"
#include "CFilterManager.h"
#include "CFont.h"
#include "CInput.h"
#include "CTable.h"

using namespace std;

class CSortFilter{
public:

  CSortFilter();
  ~CSortFilter();

  void clear      ();
  void init       (CTable* t);
  int  logic      (int mouseX, int mouseY, int mouseButton, bool mouseButton1);
  void render     ();

  void setDisplay (CDisplay* d);
  void setFocus   (CActiveFocus* f);
  void setFont    (CFont* f);
  void setInput   (CInput* i);

private:

  CActiveFocus* activeFocus;
  CDisplay*     display;
  CFont*        font;
  CInput*       input;
  CTable*       dt;

  CButton butApply;
  CButton butCancel;
  CButton butBack;

  CCheckbox cbSortAsc;

  CDropDown ddSort;

  CFilterManager fm;

};

#endif