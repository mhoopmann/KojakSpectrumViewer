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

#include <string>
#include <vector>

using namespace std;

typedef struct kvFilter {
  string colID;
  char type;    //0=numeric, 1=text
  char filter;  //0=above, 1=below, 2=between, 3=exactly, 4=contains
  double dLow;
  double dHigh;
  string sLow;
  string sHigh;
} kvFilter;

class CFilterManager {
public:

  CFilterManager();
  ~CFilterManager();

  int posX;
  int posY;

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
  vector<kvFilter> vChosen;

};

#endif