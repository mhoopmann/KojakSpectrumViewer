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
  kvColor spectrum[3];
  kvColor spectrumIons[3][6];
  kvColor text[3];
  kvColor toolbar[2];
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

#endif