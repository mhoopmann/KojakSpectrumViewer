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

#ifndef _CLIST_H
#define _CLIST_H

#include "CDisplay.h"
#include "CFont.h"
#include "CGfxCollection.h"
#include <vector>

using namespace std;

typedef struct sListItem{
  string item;
  int icon; //if non-negative, show icon from global gfx object
  int value;
} sListItem;

class CList{
public:
  CList();
  ~CList();

  int posX;
  int posY;
  int szX;
  int szY;

  sListItem& operator[](const int& index);

  bool      addItem         (char* str, int value, int icon=-1);
  void      clear           ();
  void      clearSelected   ();
  void      deleteItem      (int index);
  void      fixLayout       ();
  int       getSelected     ();
  sListItem getSelectedItem ();
  
  int   logic       (int mouseX, int mouseY, int mouseButton, bool mouseButton1);
  void  render      ();
  
  void  setDisplay  (CDisplay* d);
  void  setFont     (CFont* f);
  void  setGfx      (CGfxCollection* g);
  bool  setSelected (int index);
  void  setSize     (int sz);
  size_t  size();

private:

  CDisplay* display;
  CFont*    font;
  CGraphic* icons;

  int offset;
  int selected;
  int szFont;
  int szHeight;
  int highlight;

  vector<sListItem>* items;

  int     lastMouseY;
  double  scrollJumpV;
  bool    scrollLockV;
  int     scrollOffsetV;
  bool    showScrollbarV;
  int     thumbHeightV;
  int     thumbMaxV;

  kvColor colors[3];
  int     txtColor;

};

#endif