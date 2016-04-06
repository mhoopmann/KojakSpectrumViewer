#ifndef _PEPXMLSTRUCTS_H
#define _PEPXMLSTRUCTS_H

#include <string>

using namespace std;

typedef struct PepXMLError {
  double error;
  double prob;
} PepXMLError;

typedef struct PepXMLPepMod {
  char index;
  char pos; //0-based
} PepXMLPepMod;

typedef struct PepXMLPepScore {
  char index;
  double value;
} PepXMLPepScore;

typedef struct PepXMLXL {
  string ID;
  double mass;
} PepXMLXL;

typedef struct PepXMLMod {
  char aa;              //the amino acid, stored as letter in pepXML, returned as position when requested
  double massSearch;    //mass that was in the search parameters
  double massDiff;      //mass difference that was in the search parameters
  double massStd;       //standard representation of the searched mass
  double massDiffStd;   //standard representation of the diffential mass
  string label;         //Give it a name
} PepXMLMod;

#endif