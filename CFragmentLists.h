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

#ifndef _CFRAGMENTLISTS_H
#define _CFRAGMENTLISTS_H

#include "CDisplay.h"
#include "CFont.h"
#include "CList.h"

class CFragmentLists{
public:

  CFragmentLists();
  ~CFragmentLists();
  
  bool bShowIons[3][6];

  void    addMod    (int pos, double modMass);
  void    calcIons  (int link, int link2, double modMass);
  double  getAIon   (int ch, int index);
  double  getBIon   (int ch, int index);
  double  getCIon   (int ch, int index);
  double  getXIon   (int ch, int index);
  double  getYIon   (int ch, int index);
  double  getZIon   (int ch, int index);
  void init         ();
  bool logic        ();
  bool render       ();
  void setDisplay   (CDisplay* d);
  void setFont      (CFont* f);
  void setMatch     (int ion, int ch, int index, bool value);
  void setPeptide   (string pep, int ch, int linkPos=-1, int linkPos2=-1, double linkMass=0.0, bool n15=false);
  int  size         ();

  int posX;
  int posY;
  int szX;
  int szY;


private:

  void allocate   ();
  void deallocate ();

  CDisplay* display;
  CFont* font;

  double** aFrag;
  double** bFrag;
  double** cFrag;
  double** xFrag;
  double** yFrag;
  double** zFrag;

  bool** aMatch;
  bool** bMatch;
  bool** cMatch;
  bool** xMatch;
  bool** yMatch;
  bool** zMatch;

  double aaMass[128];
  double aaMass15n[128];
  int charge;
  int ionCount;
  bool is15N;
  int offsetX;
  int offsetY;
  string peptide;
  double peptideMass;

  kvColor colorIons[3][6];
  kvColor colorTable[3];
  int     colorTxt[3];

};

#endif