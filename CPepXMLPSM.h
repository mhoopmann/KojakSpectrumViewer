#ifndef _CPEPXMLPSM_H
#define _CPEPXMLPSM_H

#include "PepXMLStructs.h"
#include "CPepXMLPeptide.h"

using namespace std;

class CPepXMLPSM {
public:
  CPepXMLPSM();
  CPepXMLPSM(const CPepXMLPSM& p);
  ~CPepXMLPSM();

  CPepXMLPSM& operator=(const CPepXMLPSM& p);

  void clear();

  char xlIndex; //Reference to parent cross-linker table
  char xlType;  //0=not, 1=loop, 2=cross

  int rank;

  double calcPSMNeutMass;   //psm also has a neutral mass, in cases of cross-links
  double probability;
  double iProphetProbability;

  vector<PepXMLPepScore>* psmScores;

  CPepXMLPeptide* peptide;
  CPepXMLPeptide* xlPeptide;

};

#endif