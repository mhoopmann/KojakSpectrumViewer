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
  

};

#endif