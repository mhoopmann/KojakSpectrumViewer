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