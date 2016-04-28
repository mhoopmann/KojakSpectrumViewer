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

#ifndef _CPEPTIDEBOX_H
#define _CPEPTIDEBOX_H

#include "CActiveFocus.h"
#include "CCheckbox.h"
#include "CDisplay.h"
#include "CFont.h"
#include "CFragmentLists.h"
#include "Structs.h"

class CPeptideBox {
public:

  CPeptideBox();
  ~CPeptideBox();

  int posX;
  int posY;
  int szX;
  int szY;

  int charge;
  int scanNumber;
  bool clear;
  double mass;

  string metaTol;

  bool            checkSeries (int charge, int ion);
  void            fixLayout   ();
  CFragmentLists* getIonSeries(bool pepB=false);
  void            init        ();
  int             logic       (int mouseX, int mouseY, int mouseButton, bool mouseButton1);
  void            render      (int x, int y);
  void            setDisplay  (CDisplay* d);
  void            setFocus    (CActiveFocus* f);
  void            setFont     (CFont* f);
  void            setPSM      (kvPSM& psm);
  bool            showPeptide (bool alpha=true);

private:

  CActiveFocus* activeFocus;
  CDisplay*     display;
  CFont*        font;

  CCheckbox cbIons[3][6];
  CCheckbox cbPepA;
  CCheckbox cbPepB;

  CFragmentLists fragPepA;
  CFragmentLists fragPepB;

  SDL_Rect rLinkA;
  SDL_Rect rLinkB;

  string sPepA;
  string sPepB;

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

  int txtColor;

};

#endif