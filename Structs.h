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

#ifndef _KVSTRUCTS_H
#define _KVSTRUCTS_H

#include <string>
#include <vector>

using namespace std;

typedef struct kvColor {
  int r;
  int g;
  int b;
} kvColor;

typedef struct kvPalette {
  kvColor buttons[5];
  kvColor checkbox;
  kvColor ionTable[3];
  kvColor list[3];
  kvColor spectrum[3];
  kvColor spectrumIons[3][6];
  kvColor slider;
  kvColor table[4];
  kvColor textBox[2];
  kvColor toolbar[2];
  int txtButton[2];
  int txtCheckbox;
  int txtDefault;
  int txtIonTable[3];
  int txtList;
  int txtSpectrum;
  int txtTable[2];
  int txtTextBox;
} kvPalette;

typedef struct kvMod {
  int pos;
  double mass;
} kvMod;

typedef struct kvPSM {
  char charge;
  int linkA;
  int linkB;
  int linkType;   //0=na, 1=loop, 2=cross
  int scanNumber;
  int fileID;
  double compMassA;
  double compMassB;
  double mass;
  bool n15A;
  bool n15B;
  string peptideA;
  string peptideB;
  vector<kvMod>* modA;
  vector<kvMod>* modB;
  kvPSM(){
    charge=0;
    linkA=0;
    linkB=0;
    linkType=0;
    scanNumber=0;
    fileID=0;
    compMassA=0;
    compMassB=0;
    mass=0;
    n15A=false;
    n15B=false;
    peptideA.clear();
    peptideB.clear();
    modA = new vector<kvMod>;
    modB = new vector<kvMod>;
  }
  kvPSM(const kvPSM& k){
    size_t i;
    charge=k.charge;
    linkA=k.linkA;
    linkB=k.linkB;
    linkType=k.linkType;
    scanNumber=k.scanNumber;
    fileID=k.fileID;
    compMassA=k.compMassA;
    compMassB=k.compMassB;
    peptideA=k.peptideA;
    peptideB=k.peptideB;
    mass=k.mass;
    n15A=k.n15A;
    n15B=k.n15B;
    modA = new vector<kvMod>;
    modB = new vector<kvMod>;
    for(i=0;i<k.modA->size();i++) modA->push_back(k.modA->at(i));
    for(i=0;i<k.modB->size();i++) modB->push_back(k.modB->at(i));
  }
  ~kvPSM(){
    delete modA;
    delete modB;
  }
  kvPSM& operator=(const kvPSM& k){
    if(this!=&k){
      size_t i;
      charge=k.charge;
      linkA=k.linkA;
      linkB=k.linkB;
      linkType=k.linkType;
      scanNumber=k.scanNumber;
      fileID=k.fileID;
      compMassA=k.compMassA;
      compMassB=k.compMassB;
      peptideA=k.peptideA;
      peptideB=k.peptideB;
      mass=k.mass;
      n15A = k.n15A;
      n15B = k.n15B;
      delete modA;
      delete modB;
      modA = new vector<kvMod>;
      modB = new vector<kvMod>;
      for(i=0;i<k.modA->size();i++) modA->push_back(k.modA->at(i));
      for(i=0;i<k.modB->size();i++) modB->push_back(k.modB->at(i));
    }
    return *this;
  }
  void clear(){
    modA->clear();
    modB->clear();
  }

} kvPSM;

typedef struct kvTextLine {
  string text;
  int width;
} kvTextLine;

typedef struct kvFilter {
  string colID;
  char type;    //0=int, 1=double, 2=text
  char filter;  //0=above, 1=below, 2=between, 3=contains, 4=exactly
  double dLow;
  double dHigh;
  int iLow;
  int iHigh;
  string sLow;
  string sHigh;
} kvFilter;

#endif