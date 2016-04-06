#include "CPepXMLPeptide.h"

CPepXMLPeptide::CPepXMLPeptide(){
  label=0;
  nextAA='-';
  prevAA='-';
  calcPepNeutMass=0;
  complementMass=0;
  modifiedPeptide.clear();
  peptide.clear();
  mods=new vector<PepXMLPepMod>;
  proteins=new vector<size_t>;
  xlScores=new vector<PepXMLPepScore>;
}

CPepXMLPeptide::CPepXMLPeptide(const CPepXMLPeptide& p){
  label=p.label;
  nextAA=p.nextAA;
  prevAA=p.prevAA;
  calcPepNeutMass=p.calcPepNeutMass;
  complementMass=p.complementMass;
  modifiedPeptide=p.modifiedPeptide;
  peptide=p.peptide;
  mods=new vector<PepXMLPepMod>;
  proteins=new vector<size_t>;
  xlScores=new vector<PepXMLPepScore>;
  size_t i;
  for(i=0;i<p.mods->size();i++) mods->push_back(p.mods->at(i));
  for(i=0;i<p.proteins->size();i++) proteins->push_back(p.proteins->at(i));
  for(i=0;i<p.xlScores->size();i++) xlScores->push_back(p.xlScores->at(i));
}

CPepXMLPeptide::~CPepXMLPeptide(){
  delete mods;
  delete proteins;
  delete xlScores;
}

CPepXMLPeptide& CPepXMLPeptide::operator=(const CPepXMLPeptide& p){
  if(this!=&p){
    label=p.label;
    nextAA=p.nextAA;
    prevAA=p.prevAA;
    calcPepNeutMass=p.calcPepNeutMass;
    complementMass=p.complementMass;
    modifiedPeptide=p.modifiedPeptide;
    peptide=p.peptide;
    delete mods;
    delete proteins;
    delete xlScores;
    mods=new vector<PepXMLPepMod>;
    proteins=new vector<size_t>;
    xlScores=new vector<PepXMLPepScore>;
    size_t i;
    for(i=0;i<p.mods->size();i++) mods->push_back(p.mods->at(i));
    for(i=0;i<p.proteins->size();i++) proteins->push_back(p.proteins->at(i));
    for(i=0;i<p.xlScores->size();i++) xlScores->push_back(p.xlScores->at(i));
  }
  return *this;
}

void CPepXMLPeptide::clear(){
  label=0;
  nextAA='-';
  prevAA='-';
  calcPepNeutMass=0;
  complementMass=0;
  modifiedPeptide.clear();
  peptide.clear();
  delete mods;
  delete proteins;
  delete xlScores;
  mods=new vector<PepXMLPepMod>;
  proteins=new vector<size_t>;
  xlScores=new vector<PepXMLPepScore>;
}
