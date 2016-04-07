#ifndef _CPEPXMLPEPTIDE_H
#define _CPEPXMLPEPTIDE_H

#include "PepXMLStructs.h"
#include <string>
#include <vector>

using namespace std;

class CPepXMLPeptide {
public:

  CPepXMLPeptide();
  CPepXMLPeptide(const CPepXMLPeptide& p);
  ~CPepXMLPeptide();

  CPepXMLPeptide& operator=(const CPepXMLPeptide& p);

  void clear();

  char label;  //0=alpha, 1=beta : for cross-links
  char nextAA;
  char prevAA;

  double calcPepNeutMass;
  double complementMass;

  string modifiedPeptide;
  string peptide;

  vector<PepXMLPepMod>*   mods;
  vector<size_t>*         proteins;
  vector<PepXMLPepScore>* xlScores;
  
};

#endif