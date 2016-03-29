#ifndef _CTEXTBOX_H
#define _CTEXTBOX_H

#include "CActiveFocus.h"
#include "CDisplay.h"
#include "CFont.h"
#include "Structs.h"
#include <deque>

using namespace std;

class CTextBox{
public:

  CTextBox();
  ~CTextBox();

  bool  noScroll;
  int   posX;
  int   posY;
  int   szX;
  int   szY;

  void            addText     (char* str);
  void            addText     (string& str);
  void            clear       ();
  void            fixLayout   ();

  int             logic       (int mouseX, int mouseY, int mouseButton, bool mouseButton1);
  void            render      ();

  void            setDisplay  (CDisplay* d);
  void            setFocus    (CActiveFocus* f);
  void            setFont     (CFont* f);
  void            setSize     (int sz);

private:

  CActiveFocus* activeFocus;
  CDisplay*     display;
  CFont*        font;

  deque<kvTextLine> buffer;
  
  int maxWidth;
  int szFont;
  int szHeight;

  int     lastMouseY;
  double  scrollJumpV;
  bool    scrollLockV;
  int     scrollOffsetV;
  bool    showScrollbarV;
  int     thumbHeightV;
  int     thumbMaxV;

  int     lastMouseX;
  double  scrollJumpH;
  bool    scrollLockH;
  int     scrollOffsetH;
  bool    showScrollbarH;
  int     thumbHeightH;
  int     thumbMaxH;

};

#endif