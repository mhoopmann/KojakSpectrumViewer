/*
Copyright 2016, Michael R. Hoopmann, Institute for Systems Biology

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

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
#include "CSettings.h"
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
  CSettings setDlg;
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