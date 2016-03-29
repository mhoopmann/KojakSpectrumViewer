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

};

#endif