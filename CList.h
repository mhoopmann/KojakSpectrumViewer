#ifndef _CLIST_H
#define _CLIST_H

#include "CDisplay.h"
#include "CFont.h"
#include "CGfxCollection.h"
#include <vector>

using namespace std;

typedef struct sListItem{
  string item;
  int icon; //if non-negative, show icon from global gfx object
  int value;
} sListItem;

class CList{
public:
  CList();
  ~CList();

  int posX;
  int posY;
  int szX;
  int szY;

  sListItem& operator[](const int& index);

  bool      addItem         (char* str, int value, int icon=-1);
  void      clear           ();
  void      clearSelected   ();
  void      deleteItem      (int index);
  void      fixLayout       ();
  int       getSelected     ();
  sListItem getSelectedItem ();
  
  int   logic       (int mouseX, int mouseY, int mouseButton, bool mouseButton1);
  void  render      ();
  
  void  setDisplay  (CDisplay* d);
  void  setFont     (CFont* f);
  void  setGfx      (CGfxCollection* g);
  bool  setSelected (int index);
  void  setSize     (int sz);
  size_t  size();

private:

  CDisplay* display;
  CFont*    font;
  CGraphic* icons;

  int offset;
  int selected;
  int szFont;
  int szHeight;
  //int highlight;

  vector<sListItem>* items;

  int     lastMouseY;
  double  scrollJumpV;
  bool    scrollLockV;
  int     scrollOffsetV;
  bool    showScrollbarV;
  int     thumbHeightV;
  int     thumbMaxV;

};

#endif