#ifndef _CBUTTON_H
#define _CBUTTON_H

#include "CActiveFocus.h"
#include "CDisplay.h"
#include "CGfxCollection.h"
#include "CGraphic.h"
#include "CFont.h"
#include <string>

using namespace std;

class CButton{
public:
  CButton();
  ~CButton();
  
  bool  active;
  int   posX;
  int   posY;
  int   szX;
  int   szY;

  bool logic        (int mouseX, int mouseY, int mouseButton);
  void render       ();
  void setCaption   (char* str);
  void setCaption   (string str);
  void setDisplay   (CDisplay* d);
  void setFocus     (CActiveFocus* f);
  void setFont      (CFont* f);
  void setGfx       (CGfxCollection* g);
  void setImage     (int index);
  

private:

  CDisplay* display;
  CFont*    font;
  CGraphic* image;  //Image or text for button

  CActiveFocus* activeFocus;

  string  caption;
  bool    lockButton;
  bool    highlight;
  int     imgIndex;
  int     textWidth;
  int     textHeight;

  kvColor colors[5];
  int tColors[2];

};

#endif