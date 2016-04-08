#ifndef _CVIEWER_H
#define _CVIEWER_H

#include "CAbout.h"
#include "CActiveFocus.h"
#include "CButton.h"
#include "CDisplay.h"
#include "CFileOpen.h"
#include "CFont.h"
#include "CGfxCollection.h"
#include "CInput.h"
#include "CPeptideBox.h"
#include "CResults.h"
#include "CSlider.h"
#include "CSortFilter.h"
#include "CSpectrumGraph.h"
#include "CTable.h"
#include "CTextBox.h"
#include "CToolbar.h"

using namespace MSToolkit;

class CViewer{
public:

  CViewer();
  CViewer(CDisplay* d, CInput* inp);
  ~CViewer();

  void init();
  void processEvent(SDL_Event& e);
  void setDisplay(CDisplay* d);
  void setFocus(CActiveFocus* f);
  void setInput(CInput* inp);
  bool viewerMain();

private:

  CDisplay* display;
  CInput* input;

  CAbout aboutDlg;
  CFileOpen fileDlg;
  CFont font;
  CGfxCollection gfx;
  CPeptideBox pepBox;
  CSpectrumGraph sg;
  CTable dt;
  CTextBox msgBox;
  CToolbar tb;
  CSlider sliderH,sliderV;
  CSortFilter sortDlg;
  //SDL_Rect slider;

  CActiveFocus* activeFocus;

  CResults data;
  
  bool firstOpen;
  bool quitter;
  bool focused;
  int dataIndex;
  bool sliderLock;
  int lastMouseX;
  int scanNumber;
  int state;  //0=Viewer, 1=File Loading

  void fixLayout();
  bool logic();
  bool render();
  void resetSpectrum();

};

#endif