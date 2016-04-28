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

#ifndef _CSPECTRUMGRAPH_H
#define _CSPECTRUMGRAPH_H

#include "CDataSet.h"
#include "CDisplay.h"
#include "CFont.h"
#include "CFragmentLists.h"
#include "CInput.h"
#include "CPeptideBox.h"

class CSpectrumGraph{
public:

  CSpectrumGraph();
  CSpectrumGraph(CDisplay* d, CInput* inp);
  ~CSpectrumGraph();

  int posX;
  int posY;
  int szX;
  int szY;

  CDataSet spectrum;

  bool logic(int mouseX, int mouseY, int mouseButton, bool mouseButton1);
  bool render(CPeptideBox& p);
  void resetView();
  void setDisplay(CDisplay* d);
  void setFont(CFont* f);
  void setInput(CInput* inp);

private:

  int matchPeak(double d);

  CFont* font;
  CInput* input;
  CDisplay* display;

  double lowX,lowY;
  double highX,highY;
  bool zoomLock;
  int lockPos1;
  int lockPos2;

  kvColor color[3];
  kvColor colorIons[3][6];
  int     txtColor;

};

#endif