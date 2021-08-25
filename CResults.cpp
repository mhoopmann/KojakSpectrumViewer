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

#include "CResults.h"

CResults::CResults(){
  psms = new vector<kvPSM>;
  dataFiles = new vector<string>;
  dataFilesActive = new vector<string>;
  msr.setFilter(MS1);
  msr.addFilter(MS2);
  msr.addFilter(MS3);
}

CResults::~CResults(){
  delete psms;
  delete dataFiles;
  delete dataFilesActive;
}

kvPSM& CResults::operator [](const int& index){
  return psms->at(index);
}

int CResults::findSpectrum(int scanNumber){
  int sz=(int)psms->size();
  int lower=0;
  int upper=sz;
  int mid=upper/2;

  if(sz==0) return -1;

  //binary search to first mass within boundaries
  while(scanNumber!=psms->at(mid).scanNumber){
		if(lower>=upper) return -1;
    if(scanNumber<psms->at(mid).scanNumber){
			upper=mid-1;
			mid=(lower+upper)/2;
		} else {
			lower=mid+1;
			mid=(lower+upper)/2;
		}
		if(mid==sz) return -1;
	}
  return mid;
}

Spectrum& CResults::getSpectrum(const int& index){
  if(psms->at(index).fileID!=activeFile){
    activeFile=psms->at(index).fileID;
    msr.readFile(&dataFilesActive->at(activeFile)[0],spec,psms->at(index).scanNumber);
  } else {
    msr.readFile(NULL,spec,psms->at(index).scanNumber);
  }
  return spec;
}

int CResults::indexFile(string& s){
  size_t i;
  string str;

  for(i=0;i<dataFiles->size();i++){
    if(s.compare(dataFiles->at(i))==0) return i;
  }

  //Try opening the file
  msr.readFile(&s[0],spec);
  if(spec.getScanNumber()>0){
    dataFiles->push_back(s);
    dataFilesActive->push_back(s);
    activeFile = (int) dataFilesActive->size()-1;
    return activeFile;
  }

  //Remove path and try opening in CWD
  str=s.substr(s.rfind('/')+1,s.size());
  if(str.compare(s)==0) str=s.substr(s.rfind('\\')+1,s.size());
  msr.readFile(&str[0],spec);
  if(spec.getScanNumber()>0){
    dataFiles->push_back(s);
    dataFilesActive->push_back(str);
    activeFile = (int) dataFilesActive->size()-1;
    return activeFile;
  }

  //report failue upon reaching this point
  return -1;
}

int CResults::readPepXML(char* fn, CTable* t){

  PepXMLParser3 p;
  PepXMLPepMod mod;
  kvPSM psm;
  kvMod m;
  size_t i,k;
  size_t index;
  char a,b;
  char str[256];
  string s;
  kvTableDP dp;
  string n15tag;

  //Clear any previous data
  psms->clear();
  dataFiles->clear();
  dataFilesActive->clear();

  p.readFile(fn);
  n15tag = p.getParameter("15N_filter");
  if(n15tag.size()==0) n15tag="nullNULL";
  for(i=0;i<p.size();i++){
    if(p[i].psms->size()==0) continue;
    psm.clear();
    psm.scanNumber=p[i].scanNumber;
    psm.mass=p[i].precursorNeutMass;
    psm.charge=p[i].charge;
    psm.peptideA=p.getPeptide(i,false);
    psm.peptideB=p.getPeptide(i,false,1,true);
    for(k=0;k<p.getPeptideModCount(i);k++){
      mod=p.getPeptideMod(i,k);
      m.pos=mod.pos;
      m.mass=mod.massDiff;
      psm.modA->push_back(m);
    }
    for(k=0;k<p.getPeptideModCount(i,1,true);k++){
      mod=p.getPeptideMod(i,k,1,true);
      m.pos=mod.pos;
      m.mass=mod.massDiff;
      psm.modB->push_back(m);
    }
    switch(p.getLinkType(i)){
      case 1:
        psm.linkType=1;
        p.getLinkSites(i,a,b);
        psm.linkA=(int)a-1;
        psm.linkB=(int)b-1;
        psm.compMassA=p.getLinker(i).mass;
        psm.compMassB=0;
        break;
      case 2:
        psm.linkType=2;
        p.getLinkSites(i,a,b);
        psm.linkA=(int)a-1;
        psm.linkB=(int)b-1;
        psm.compMassA=p[i][0].calcPSMNeutMass-p[i][0].peptide->calcPepNeutMass;
        psm.compMassB=p[i][0].calcPSMNeutMass-p[i][0].xlPeptide->calcPepNeutMass;
        break;
      default:
        psm.linkType=0;
        psm.linkA=-1;
        psm.linkB=-1;
        psm.compMassA=0;
        psm.compMassB=0;
        break;
    }
    s=p.getFile(i);
    psm.fileID=indexFile(s);
    if(psm.fileID<0){
      //cannot load corresponding spectrum data file
      return -1;
    }
    psms->push_back(psm);

    //Add PSM to table
    index=psms->size()-1;
    t->addRow(index);

    //Scan number
    dp.clear();
    dp.ID=t->getColumn("Scan");
    if(dp.ID==-1) dp.ID=t->addColumn("Scan",0,true);
    dp.iVal=p[i].scanNumber;
    t->addDataPoint(index,dp);

    //Probability - Regular if not iProphet
    if(p.hasIProphet()){
      dp.clear();
      dp.ID=t->getColumn("iProb");
      if(dp.ID==-1) dp.ID=t->addColumn("iProb", 1, true);
      dp.dVal=p[i][0].iProphetProbability;
      t->addDataPoint(index, dp);
    } else if(p.hasPepProphet()){
      dp.clear();
      dp.ID=t->getColumn("prob");
      if(dp.ID==-1) dp.ID=t->addColumn("prob", 1, true);
      dp.dVal=p[i][0].probability;
      t->addDataPoint(index, dp);
    }

    //Precursor Mass
    dp.clear();
    dp.ID=t->getColumn("PreMass");
    if(dp.ID==-1) dp.ID=t->addColumn("PreMass",1,true);
    dp.dVal=p[i].precursorNeutMass;
    t->addDataPoint(index,dp);

    //Charge
    dp.clear();
    dp.ID=t->getColumn("Ch");
    if(dp.ID==-1) dp.ID=t->addColumn("Ch",0,true);
    dp.iVal=p[i].charge;
    t->addDataPoint(index,dp);

    //Search-specific scores
    dp.clear();
    for(k=0;k<p[i][0].psmScores->size();k++){
      dp.ID=t->getColumn(p.getScoreLabel(p[i][0].psmScores->at(k).index));
      if(dp.ID==-1) dp.ID=t->addColumn(p.getScoreLabel(p[i][0].psmScores->at(k).index),1,true);
      dp.dVal=p[i][0].psmScores->at(k).value;
      t->addDataPoint(index,dp);
    }

    //Peptide (alpha)
    dp.clear();
    dp.ID=t->getColumn("Peptide");
    if(dp.ID==-1) dp.ID=t->addColumn("Peptide",2,true);
    dp.sVal=p[i][0].peptide->prevAA;
    dp.sVal+='.';
    dp.sVal+=p.getPeptide(i,true);
    dp.sVal+='.';
    dp.sVal+=p[i][0].peptide->nextAA;
    t->addDataPoint(index,dp);

    //Protein (alpha)
    dp.ID=t->getColumn("Protein");
    if(dp.ID==-1) dp.ID=t->addColumn("Protein",2,true);
    dp.sVal=p.getProtein(i,0);
    if(dp.sVal.find(n15tag)==0) psms->back().n15A=true;
    if(p[i][0].peptide->proteins->size()>1){
      sprintf(str," +%d",(int)p[i][0].peptide->proteins->size()-1);
      dp.sVal+=str;
    }
    t->addDataPoint(index,dp);

    //beta
    if(p[i][0].xlType==2){
      dp.clear();
      //Peptide (beta)
      dp.ID=t->getColumn("PeptideB");
      if(dp.ID==-1) dp.ID=t->addColumn("PeptideB",2,true);
      dp.sVal=p[i][0].xlPeptide->prevAA;
      dp.sVal+='.';
      dp.sVal+=p.getPeptide(i,true,1,true);
      dp.sVal+='.';
      dp.sVal+=p[i][0].xlPeptide->nextAA;
      t->addDataPoint(index,dp);

      //Protein (beta)
      dp.ID=t->getColumn("ProteinB");
      if(dp.ID==-1) dp.ID=t->addColumn("ProteinB",2,true);
      dp.sVal=p.getProtein(i,0,1,true);
      if (dp.sVal.find(n15tag) ==0) psms->back().n15B = true;
      if(p[i][0].xlPeptide->proteins->size()>1){
        sprintf(str," +%d",(int)p[i][0].xlPeptide->proteins->size()-1);
        dp.sVal+=str;
      }
      t->addDataPoint(index,dp);
    }

    //link sites && cross-linker
    if(p[i][0].xlType>0){
      dp.clear();
      p.getLinkSites(i,a,b);
      //LinkA
      dp.ID=t->getColumn("LinkA");
      if(dp.ID==-1) dp.ID=t->addColumn("LinkA",0,true);
      dp.iVal=(int)a;
      t->addDataPoint(index,dp);
      //LinkB
      dp.ID=t->getColumn("LinkB");
      if(dp.ID==-1) dp.ID=t->addColumn("LinkB",0,true);
      dp.iVal=(int)b;
      t->addDataPoint(index,dp);
      
      //linker
      dp.clear();
      dp.ID=t->getColumn("Linker");
      if(dp.ID==-1) dp.ID=t->addColumn("Linker",2,true);
      dp.sVal=p.getLinker(i).ID;
      t->addDataPoint(index,dp);
    }
    
  }

  t->fixLayout();
  return 0;
}

size_t CResults::size(){
  return psms->size();
}

