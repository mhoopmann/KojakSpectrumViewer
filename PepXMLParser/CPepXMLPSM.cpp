#include "CPepXMLPSM.h"  

CPepXMLPSM::CPepXMLPSM(){
  xlIndex=0;
  xlType=0;
  rank=0;
  calcPSMNeutMass=0;
  probability=0;
  iProphetProbability=0;
  peptide=NULL;
  xlPeptide=NULL;
  psmScores = new vector<PepXMLPepScore>;
}

CPepXMLPSM::CPepXMLPSM(const CPepXMLPSM& p){
  xlIndex=p.xlIndex;
  xlType=p.xlType;
  rank=p.rank;
  calcPSMNeutMass=p.calcPSMNeutMass;
  probability=p.probability;
  iProphetProbability=p.iProphetProbability;
  peptide=NULL;
  if(p.peptide!=NULL) peptide = new CPepXMLPeptide(*p.peptide);
  xlPeptide=NULL;
  if(p.xlPeptide!=NULL) xlPeptide= new CPepXMLPeptide(*p.xlPeptide);
  size_t i;
  psmScores = new vector<PepXMLPepScore>;
  for(i=0;i<p.psmScores->size();i++) psmScores->push_back(p.psmScores->at(i));  
}

CPepXMLPSM::~CPepXMLPSM(){
  delete psmScores;
  if(peptide!=NULL) delete peptide;
  if(xlPeptide!=NULL) delete xlPeptide;
}

CPepXMLPSM& CPepXMLPSM::operator=(const CPepXMLPSM& p){
  if(this!=&p){
    if(peptide!=NULL) delete peptide;
    if(xlPeptide!=NULL) delete xlPeptide;
    delete psmScores;

    xlIndex=p.xlIndex;
    xlType=p.xlType;
    rank=p.rank;
    calcPSMNeutMass=p.calcPSMNeutMass;
    probability=p.probability;
    iProphetProbability=p.iProphetProbability;
    peptide=NULL;
    xlPeptide=NULL;
    if(p.peptide!=NULL) peptide = new CPepXMLPeptide(*p.peptide);
    if(p.xlPeptide!=NULL) xlPeptide= new CPepXMLPeptide(*p.xlPeptide);

    size_t i; 
    psmScores = new vector<PepXMLPepScore>;
    for(i=0;i<p.psmScores->size();i++) psmScores->push_back(p.psmScores->at(i));  
  }
  return *this;
}

void CPepXMLPSM::clear(){
  xlIndex=0;
  xlType=0;
  rank=0;
  calcPSMNeutMass=0;
  probability=0;
  iProphetProbability=0;
  if(peptide!=NULL) delete peptide;
  if(xlPeptide!=NULL) delete xlPeptide;
  peptide=NULL;
  xlPeptide=NULL;
  delete psmScores;
  psmScores = new vector<PepXMLPepScore>;
}
