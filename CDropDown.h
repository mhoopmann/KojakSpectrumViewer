#ifndef _CDROPDOWN_H
#define _CDROPDOWN_H

#include "CActiveFocus.h"
#include "CButton.h"
#include "CDisplay.h"
#include "CFont.h"
#include <string>
#include <vector>

using namespace std;

class CDropDown{
public:
  CDropDown();
  ~CDropDown();

  bool active;

  int posX;
  int posY;
  int szX;
  int szDrop;

  int selected;
  int highlight;
  
  void    addItem     (char* str);
  void    addItem     (string& s);
  void    clear       ();
  void    fixLayout   ();
  string  getSelected ();
  int     logic       (int mouseX, int mouseY, int mouseButton, bool mouseButton1);
  void    render      ();
  void    setDisplay  (CDisplay* d);
  void    setFocus    (CActiveFocus* f);
  void    setFont     (CFont* f);
  void    setFontSize (int sz);
  size_t  size        ();

private:

  CActiveFocus* activeFocus;
  CDisplay* display;
  CFont* font;

  CButton button;

  vector<string> items;

  bool dropped;

  int     fontSize;
  int     lastMouseY;
  double  scrollJumpV;
  bool    scrollLockV;
  int     scrollOffsetV;
  bool    showScrollbarV;
  int     szY;
  int     thumbHeightV;
  int     thumbMaxV;

  kvColor colors[2];

};

#endif