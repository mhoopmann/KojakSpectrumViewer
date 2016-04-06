#ifndef _PEPXMLPARSER3_H
#define _PEPXMLPARSER3_H

#include "expat.h"
#include "PepXMLStructs.h"
#include "CPepXMLSpectrum.h"
#include <iostream>
#include <cmath>

#define XMLCLASS		
#ifndef XML_STATIC
#define XML_STATIC	// to statically link the expat libraries
#endif

using namespace std;

class PepXMLParser3 {

public:
	PepXMLParser3();
	~PepXMLParser3();

	CPepXMLSpectrum& operator[ ](const size_t& i);

	void characters(const XML_Char *s, int len);
	void endElement(const XML_Char *el);
	void startElement(const XML_Char *el, const XML_Char **attr);

  //Assuming functions are usually index-centric (i.e. will be given a peptide array index)

  string      getFile               (int pepIndex);
  int         getFileCount          ();
  void        getFileFromList       (int index, char* str);
  PepXMLXL    getLinker             (size_t index, char rank=1);
  bool        getLinkSites          (size_t index, char& a, char& b, char rank=1);
  char        getLinkType           (size_t index, char rank=1);
  string      getPeptide            (size_t index, bool mod=false, char rank=1, bool link=false);
  PepXMLMod   getPeptideMod         (size_t pepIndex, size_t modIndex, char rank=1, bool link=false);
  size_t      getPeptideModCount    (size_t index, char rank=1, bool link=false);
  PepXMLMod   getPeptideModFromList (int modIndex);
  double      getProbability        (double err);
  string      getProtein            (size_t index, size_t protIndex, char rank=1, bool link=false);
  string      getProteinFromList    (int protIndex);
  char        getScoreIndex         (string label);
  string      getScoreLabel         (char scoreIndex);

  bool    hasIProphet     ();
  bool    hasPepProphet   ();

	bool    readFile        (const char* fileName);

	size_t  size            ();
  void    sortModPep      ();
  void    sortScanNum     ();

	// -----------------------------------------------------------------------
	//  SAXHandler helper functions
	// -----------------------------------------------------------------------
	inline const char* getAttrValue(const char* name, const XML_Char **attr) {
		for (int i = 0; attr[i]; i += 2) {
			if (isAttr(name, attr[i])) return attr[i + 1];
		}
		return "";
	}
	inline bool isAttr(const char *n1, const XML_Char *n2) {	return (strcmp(n1, n2) == 0); }
	inline bool isElement(const char *n1, const XML_Char *n2)	{	return (strcmp(n1, n2) == 0); }

protected:

  bool                  bIProphet;    //Indicates data has been iPropheted
  bool                  bPepProphet;  //Indicates data has been PeptidePropheted

	CPepXMLSpectrum 			spec;
	XML_Parser						parser;
  CPepXMLPeptide        peptide;
  CPepXMLPSM            psm;

  int                   currentFileID;
  
  //Internal, global lists
  vector<PepXMLError>     vError;
  vector<string>          vFiles;
  vector<PepXMLMod>       vMods;
  vector<string>          vProteins;
  vector<string>          vScores;
  vector<CPepXMLSpectrum> vSpectra;
  vector<PepXMLXL>        vXL;

  bool    addMod        (char aa, double mass, double massDiff=0);
  double  calcMonoMass  (char *seq, bool water=true);
  char    findMod       (char aa, double mass);
  size_t  findProtein   (string& s);
  char    findScore     (string& s);
  char    findXL        (string& s, double mass);

  static int compareModPep  (const void *p1, const void *p2); 
  static int compareScanNum (const void *p1, const void *p2); 

};

#endif 