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

#ifndef _CABOUT_H
#define _CABOUT_H

#include "CActiveFocus.h"
#include "CButton.h"
#include "CDisplay.h"
#include "CFont.h"

using namespace std;

class CAbout{
public:

  CAbout();
  ~CAbout();

  void init(char* ver, char* bDate);
  int  logic(int mouseX, int mouseY, int mouseButton, bool mouseButton1);
  void render();

  void setDisplay(CDisplay* d);
  void setFocus(CActiveFocus* f);
  void setFont(CFont* f);

private:

  CActiveFocus* activeFocus;
  CDisplay*     display;
  CFont*        font;

  CButton butBack;

  string version;
  string bdate;

};

#endif