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