#ifndef _CRESULTS_H
#define _CRESULTS_H

#include "CTable.h"
#include "MSReader.h"
#include "PepXMLParser3.h"
#include "Structs.h"

using namespace MSToolkit;

class CResults {
public:

  CResults();
  ~CResults();

  kvPSM& operator[](const int& index);

  int       findSpectrum(int scanNum);
  Spectrum& getSpectrum (const int& index);
  int       readPepXML  (char* fn, CTable* t);
  size_t    size        ();

private:

  int     activeFile;
  int     indexFile(string& s);

  MSReader      msr;
  Spectrum      spec;

  vector<kvPSM>*  psms;
  vector<string>* dataFiles;
  vector<string>* dataFilesActive;
};

#endif