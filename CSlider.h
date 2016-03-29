#ifndef _CSLIDER_H
#define _CSLIDER_H

#include "CActiveFocus.h"
#include "CDisplay.h"

class CSlider{
public:
  int posX;
  int posY;
  int szX;
  int szY;
  bool vertical;

  CSlider();
  ~CSlider();

  bool  checkLocked();
  int   logic(int mouseX, int mouseY, int mouseButton, bool mouseButton1);
  bool  render();
  void  setDisplay(CDisplay* d);
  void  setFocus(CActiveFocus* f);

private:

  CActiveFocus* activeFocus;
  CDisplay* display;

  bool sliderLock;
  int lastMouse;

  kvColor color;

};

#endif