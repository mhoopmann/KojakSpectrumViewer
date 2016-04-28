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

#ifndef _CFILTERMANAGER_H
#define _CFILTERMANAGER_H

#include "CActiveFocus.h"
#include "CButton.h"
#include "CDisplay.h"
#include "CDropDown.h"
#include "CEditBox.h"
#include "CFont.h"
#include "CInput.h"
#include "CTable.h"
#include "Structs.h"

#include <string>
#include <vector>

using namespace std;

class CFilterManager {
public:

  CFilterManager();
  ~CFilterManager();

  int posX;
  int posY;

  vector<kvFilter> vChosen;

  void clear();
  void init(CTable* t);
  int  logic(int mouseX, int mouseY, int mouseButton, bool mouseButton1);
  int  processInput();
  void render();
  void setDisplay(CDisplay* d);
  void setFocus(CActiveFocus* f);
  void setFont(CFont* f);
  void setInput(CInput* i);

private:

  CActiveFocus* activeFocus;
  CDisplay*     display;
  CFont*        font;
  CInput*       input;

  CButton   butAdd;
  CDropDown item;
  CDropDown filter;
  CEditBox  valueA;
  CEditBox  valueB;

  vector<kvFilter> vAvail;
  vector<CButton>  vButDel;

};

#endif