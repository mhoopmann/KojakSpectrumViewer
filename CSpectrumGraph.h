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
#include "savepng.h"
#include <ctime>
#include <iostream>

class CSpectrumGraph{
public:

  CSpectrumGraph();
  CSpectrumGraph(CDisplay* d, CInput* inp);
  ~CSpectrumGraph();

  int posX;
  int posY;
  
  int fontSize;
  int lineWidth;

  CDataSet spectrum;

  int getSizeX();
  int getSizeY();
  void exportPNG();
  bool logic(int mouseX, int mouseY, int mouseButton, bool mouseButton1);
  bool render(CPeptideBox& p);
  void resetView();
  void resize(int sizeX, int sizeY);
  void setDisplay(CDisplay* d);
  void setFont(CFont* f);
  void setInput(CInput* inp);

private:

  bool findSpace(int startX, int startY, int width, int height, int& x, int& y);
  void markGrid(int x1, int y1, int x2, int y2);
  int matchPeak(double d);
  void resetGrid();

  CFont* font;
  CInput* input;
  CDisplay* display;
  bool** textGrid;

  int szX;
  int szY;

  double lowX,lowY;
  double highX,highY;
  bool zoomLock;
  int lockPos1;
  int lockPos2;
  int gridX,gridY;
  int gridSize;

  kvColor color[3];
  kvColor colorIons[3][6];
  int     txtColor;

};

#endif