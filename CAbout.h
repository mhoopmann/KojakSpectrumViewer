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

  void init();
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

};

#endif