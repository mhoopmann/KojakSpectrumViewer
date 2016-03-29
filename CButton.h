#ifndef _CBUTTON_H
#define _CBUTTON_H

#include "CActiveFocus.h"
#include "CDisplay.h"
#include "CFont.h"
#include <string>

using namespace std;

class CButton{
public:
  CButton();
  ~CButton();

  SDL_Texture* texture;  //Image or text for button
  
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
  

private:

  CDisplay* display;
  CFont*    font;

  CActiveFocus* activeFocus;

  string  caption;
  bool    lockButton;
  bool    highlight;
  int     textWidth;
  int     textHeight;

  kvColor colors[5];
  int tColors[2];

};

#endif