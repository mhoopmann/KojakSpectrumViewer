#ifndef _CSETTINGS_H
#define _CSETTINGS_H

#include "CActiveFocus.h"
#include "CButton.h"
#include "CCheckbox.h"
#include "CDisplay.h"
#include "CDropDown.h"
#include "CEditBox.h"
#include "CFont.h"
#include "CInput.h"

using namespace std;

class CSettings{
public:

  CSettings();
  ~CSettings();

  double  tol;
  char    tolUnit;

  void init();
  int  logic(int mouseX, int mouseY, int mouseButton, bool mouseButton1);
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

  CButton butApply;
  CButton butCancel;

  CDropDown ddTolUnit;
  CEditBox  ebTol;

};

#endif