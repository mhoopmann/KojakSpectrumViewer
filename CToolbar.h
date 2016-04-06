#ifndef _CTOOLBAR_H
#define _CTOOLBAR_H

#include "CActiveFocus.h"
#include "CButton.h"
#include "CDisplay.h"
#include "CEditBox.h"
#include "CFont.h"
#include "CGfxCollection.h"
#include "CInput.h"
#include <vector>

using namespace std;

class CToolbar {
public:
  CToolbar();
  ~CToolbar();
  
  int   posX;
  int   posY;
  int   szX;
  int   szY;

  void addSeparator (int posX, int posY);
  void init         ();
  int  logic        (int mouseX, int mouseY, int mouseButton);
  int  processInput ();
  void render       ();
  void setDisplay   (CDisplay* d);
  void setFocus     (CActiveFocus* f);
  void setFont      (CFont* f);
  void setGfx       (CGfxCollection* g);
  void setInput     (CInput* i);  

  int scanNumber;

private:

  CDisplay* display;
  CFont*    font;
  CInput*   input;

  CActiveFocus* activeFocus;

  CButton   openButton;
  CButton   prevButton;
  CButton   nextButton;
  CButton   sfButton;
  //CEditBox  jumpBox;

  vector<SDL_Rect> separators;

  bool    lockButton;

  kvColor colors[2];

};

#endif