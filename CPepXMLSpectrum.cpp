#include "CPepXMLSpectrum.h"

CPepXMLSpectrum::CPepXMLSpectrum(){
  charge=0;
  fileID=0;
  scanEnd=0;
  scanNumber=0;
  scanStart=0;
  rTimeSec=0;
  precursorNeutMass=0;
  ID.clear();
  psms = new vector<CPepXMLPSM>;
}

CPepXMLSpectrum::CPepXMLSpectrum(const CPepXMLSpectrum& p){
  charge=p.charge;
  fileID=p.fileID;
  scanEnd=p.scanEnd;
  scanNumber=p.scanNumber;
  scanStart=p.scanStart;
  rTimeSec=p.rTimeSec;
  precursorNeutMass=p.precursorNeutMass;
  ID=p.ID;
  psms = new vector<CPepXMLPSM>;
  for(size_t i=0;i<p.psms->size();i++) psms->push_back(p.psms->at(i));
}

CPepXMLSpectrum::~CPepXMLSpectrum(){
  delete psms;
}

CPepXMLSpectrum& CPepXMLSpectrum::operator=(const CPepXMLSpectrum& p){
  if(this!=&p){
    charge=p.charge;
    fileID=p.fileID;
    scanEnd=p.scanEnd;
    scanNumber=p.scanNumber;
    scanStart=p.scanStart;
    rTimeSec=p.rTimeSec;
    precursorNeutMass=p.precursorNeutMass;
    ID=p.ID;
    delete psms;
    psms = new vector<CPepXMLPSM>;
    for(size_t i=0;i<p.psms->size();i++) psms->push_back(p.psms->at(i));
  }
  return *this;
}

CPepXMLPSM& CPepXMLSpectrum::operator [](const size_t &index){
  return psms->at(index);
}

void CPepXMLSpectrum::clear(){
  charge=0;
  fileID=0;
  scanEnd=0;
  scanNumber=0;
  scanStart=0;
  rTimeSec=0;
  precursorNeutMass=0;
  ID.clear();
  delete psms;
  psms = new vector<CPepXMLPSM>;
}

size_t CPepXMLSpectrum::size(){
  return psms->size();
}

