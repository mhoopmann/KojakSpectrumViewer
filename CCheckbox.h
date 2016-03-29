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

  string caption;
  
  bool logic(int mouseX, int mouseY, int mouseButton);
  void render();
  void setDisplay(CDisplay* d);
  void setFocus(CActiveFocus* f);
  void setFont(CFont* f);
  void setSize(int s);

private:

  CActiveFocus* activeFocus;
  CDisplay* display;
  CFont* font;

  int sz;
  

};

#endif