#ifndef _CEDITBOX_H
#define _CEDITBOX_H

#include "CActiveFocus.h"
#include "CDisplay.h"
#include "CFont.h"
#include "CInput.h"
#include <string>

using namespace std;

class CEditBox {
public:
  CEditBox();
  ~CEditBox();
  
  int   posX;
  int   posY;
  int   szX;
  int   szY;

  bool logic        (int mouseX, int mouseY, int mouseButton);
  int  processInput ();
  void render       ();
  void setDisplay   (CDisplay* d);
  void setFocus     (CActiveFocus* f);
  void setFont      (CFont* f);
  void setInput     (CInput* i);

private:

  CDisplay* display;
  CFont*    font;
  CInput*   input;

  CActiveFocus* activeFocus;

  string  content;
  int     cursor;
  bool    lockButton;
  bool    lockKey[13];
  Uint32  timer;

};

#endif