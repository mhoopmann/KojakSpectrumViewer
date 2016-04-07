#include "PepXMLParser3.h"

// *******************************
//    XML Parser Code
// *******************************

// Static callback handlers
static void PepXMLParser3_startElementCallback(void *data, const XML_Char *el, const XML_Char **attr) {
	((PepXMLParser3*) data)->startElement(el, attr);
}

static void PepXMLParser3_endElementCallback(void *data, const XML_Char *el){
	((PepXMLParser3*) data)->endElement(el);
}

static void PepXMLParser3_charactersCallback(void *data, const XML_Char *s, int len){
	((PepXMLParser3*) data)->characters(s, len);
}

void PepXMLParser3::characters(const XML_Char *s, int len) {
	//nothing is held here.
}

void PepXMLParser3::endElement(const XML_Char *el) {

	string s;
  char str[32];
	size_t i,j;

  if (isElement("linked_peptide",el)){

    if(psm.peptide==NULL) {
      psm.peptide = new CPepXMLPeptide;
      *psm.peptide=peptide;
    } else {
      if(psm.xlPeptide==NULL) psm.xlPeptide = new CPepXMLPeptide;
      *psm.xlPeptide=peptide;
    }
    peptide.clear();

  } else if(isElement("modification_info",el)) {
    
    //Build out the modified peptide
    peptide.modifiedPeptide.clear();
    for(i=0;i<peptide.peptide.size();i++){
      peptide.modifiedPeptide+=peptide.peptide[i];
      for(j=0;j<peptide.mods->size();j++){
        if(peptide.mods->at(j).pos==i){
          sprintf(str,"[%.0lf]",vMods[peptide.mods->at(j).index].massSearch);
          peptide.modifiedPeptide+=str;
        }
      }
    }

  } else if(isElement("search_hit", el))	{

    if(psm.xlType<2) {
      if(psm.peptide==NULL) psm.peptide = new CPepXMLPeptide;
      *psm.peptide=peptide;
    }
    spec.psms->push_back(psm);
    peptide.clear();
    psm.clear();

	} else if(isElement("spectrum_query", el))	{

    vSpectra.push_back(spec);
    spec.clear();

	}

}


void PepXMLParser3::startElement(const XML_Char *el, const XML_Char **attr){

  char          c;
  double        d1,d2;
  string        s;
	size_t        u;

  if (isElement("aminoacid_modification",el)){
    s = getAttrValue("aminoacid",attr);
    c = s[0];
    s = getAttrValue("mass",attr);
    d1 = atof(&s[0]);
    s = getAttrValue("massdiff",attr);
    d2 = atof(&s[0]);
    addMod(c,d1,d2);

  } else if (isElement("error_point",el)){
    PepXMLError e;
    s = getAttrValue("error",attr);
    e.error=atof(&s[0]);
    s = getAttrValue("min_prob",attr);
    e.prob=atof(&s[0]);
    vError.push_back(e);

	} else if (isElement("interprophet_result",el)){
		s = getAttrValue("probability", attr);
		psm.iProphetProbability=atof(&s[0]);

  } else if(isElement("interprophet_summary",el)){
    bIProphet=true;

  } else if (isElement("linked_peptide",el)){

    s = getAttrValue("calc_neutral_pep_mass", attr);
    peptide.calcPepNeutMass=atof(&s[0]);
		peptide.peptide = getAttrValue("peptide", attr);
    s = getAttrValue("protein", attr);
    peptide.proteins->push_back(findProtein(s));
    s = getAttrValue("peptide_prev_aa", attr);
    if(s.length()>0) peptide.prevAA=s[0];
    else peptide.prevAA='-';
    s = getAttrValue("peptide_next_aa", attr);
    if(s.length()>0) peptide.nextAA=s[0];
    else peptide.nextAA='-';
    s = getAttrValue("complement_mass", attr);
    peptide.complementMass=atof(&s[0]);
    s = getAttrValue("designation", attr);
    if(s.compare("alpha")==0) peptide.label=0;
    else peptide.label=1;

  } else if( isElement("modification_info",el)){

    PepXMLPepMod pm;
    s = getAttrValue("mod_nterm_mass",attr);  
    if(s.size()>0){
      d1 = atof(&s[0]);
      pm.index = findMod('n',d1);
      pm.pos = 0;
      peptide.mods->push_back(pm);
    }
    s = getAttrValue("mod_cterm_mass",attr);  
    if(s.size()>0){
      d1 = atof(&s[0]);
      pm.index = findMod('c',d1);
      pm.pos = (char)peptide.peptide.size()-1;
      peptide.mods->push_back(pm);
    }

	} else if (isElement("mod_aminoacid_mass",el)){

    PepXMLPepMod pm;
    s = getAttrValue("mass",attr);
		d1 = atof(&s[0]);
    s = getAttrValue("position",attr);
    pm.pos = (char)atoi(&s[0])-1;
    c = peptide.peptide[pm.pos];
    pm.index=findMod(c,d1);
    peptide.mods->push_back(pm);

  } else if (isElement("msms_run_summary",el)){

    s = getAttrValue("base_name",attr);
    s += getAttrValue("raw_data",attr);
    for(u=0;u<vFiles.size();u++){
      if(s.compare(vFiles[u])==0) break;
    }
    currentFileID=(int)u;
    if(u==vFiles.size()) vFiles.push_back(s);
    cout << vFiles.size() << " Current ID: " << u << endl;

	} else if (isElement("peptideprophet_result",el)){

		s = getAttrValue("probability", attr);
		psm.probability=atof(&s[0]);

	} else if (isElement("search_hit",el)) {

		s = getAttrValue("hit_rank", attr);
		psm.rank = atoi(&s[0]);
    s = getAttrValue("xlink_type",attr);
    if(s.length()>0 && s.compare("xl")==0) {
      psm.xlType=2;
      s = getAttrValue("calc_neutral_pep_mass", attr);
      psm.calcPSMNeutMass=atof(&s[0]);
    } else { //only grab peptide info if non-linked or loop-linked
      if(s.length()>0 && s.compare("loop")==0) psm.xlType=1;
      else psm.xlType=0;
		  s = getAttrValue("calc_neutral_pep_mass", attr);
      peptide.calcPepNeutMass=atof(&s[0]);
      psm.calcPSMNeutMass=peptide.calcPepNeutMass;
		  peptide.peptide = getAttrValue("peptide", attr);
      s = getAttrValue("protein", attr);
      peptide.proteins->push_back(findProtein(s));
      s = getAttrValue("peptide_prev_aa", attr);
      if(s.length()>0) peptide.prevAA=s[0];
      else peptide.prevAA='-';
      s = getAttrValue("peptide_next_aa", attr);
      if(s.length()>0) peptide.nextAA=s[0];
      else peptide.nextAA='-';
    }

	} else if (isElement("search_score",el)) {

    PepXMLPepScore ps;
		s = getAttrValue("name", attr);
    ps.index = findScore(s);
    s = getAttrValue("value", attr);
    ps.value = atof(&s[0]);
    psm.psmScores->push_back(ps);

  } else if (isElement("search_summary",el)) {

	} else if (isElement("spectrum_query",el)) {

		s = getAttrValue("start_scan", attr);
    spec.scanStart = atoi(&s[0]);
    spec.scanNumber = spec.scanStart;  //This might not always be true
    s = getAttrValue("end_scan", attr);
    spec.scanEnd = atoi(&s[0]);
    s = getAttrValue("precursor_neutral_mass", attr);
    spec.precursorNeutMass = atof(&s[0]);
		s = getAttrValue("assumed_charge", attr);
		spec.charge = atoi(&s[0]);
		s = getAttrValue("retention_time_sec", attr);
    spec.rTimeSec = (float)atof(&s[0]);
    spec.ID = getAttrValue("spectrum", attr);
    spec.fileID = currentFileID;
    //s = getAttrValue("index", attr);
    //spec.index = atoi(&s[0]);

	} else if (isElement("terminal_modification",el)){

    s = getAttrValue("terminus",attr);
    if(s[0]=='N') c='n';
    else c='c';
    s = getAttrValue("mass",attr);
    d1 = atof(&s[0]);
    s = getAttrValue("massdiff",attr);
    d2 = atof(&s[0]);
    addMod(c,d1,d2);

  } else if (isElement("xlink",el)){

    s = getAttrValue("mass", attr);
    d1 = atof(&s[0]);
    s = getAttrValue("identifier", attr);
    psm.xlIndex = findXL(s,d1);

  } else if (isElement("xlink_score",el)) {

    PepXMLPepScore ps;
		s = getAttrValue("name", attr);
    ps.index = findScore(s);
    s = getAttrValue("value", attr);
    ps.value = atof(&s[0]);
    peptide.xlScores->push_back(ps);

  }
}

PepXMLParser3::PepXMLParser3(){
  bIProphet   = false;
  bPepProphet = false;
	parser      = XML_ParserCreate(NULL);
	XML_SetUserData(parser, this);
	XML_SetElementHandler(parser, PepXMLParser3_startElementCallback, PepXMLParser3_endElementCallback);
	XML_SetCharacterDataHandler(parser, PepXMLParser3_charactersCallback);
}

PepXMLParser3::~PepXMLParser3(){
	XML_ParserFree(parser);
}

CPepXMLSpectrum& PepXMLParser3::operator[ ](const size_t& i){
  return vSpectra[i];
}

bool PepXMLParser3::addMod(char aa, double mass, double massDiff){
  size_t i;
  PepXMLMod mod;
  
  mod.aa=aa;
  mod.massSearch=mass;
  mod.massDiff=massDiff;
  mod.massDiffStd=0;
  mod.massStd=0;
  mod.label.clear();

  //Look up actual modification mass: these need to be adjusted!!
  switch(aa){
    case 'C':
      if(fabs(mass-160.0307)<0.001) {
        mod.massDiffStd=57.021464;
        mod.massStd=calcMonoMass("C",false)+57.021464;
        mod.label="Carbamidomethyl_C";
      }
      if(fabs(mass-143.0042)<0.001) {
        mod.massDiffStd=39.9949141;
        mod.massStd=calcMonoMass("C",false)+39.9949141;
        mod.label="Carbamidomethyl_C-NH3";
      }
      break;
    case 'E':
      if(fabs(mass-111.0320)<0.001) {
        mod.massDiffStd=-18.0105633;
        mod.massStd=calcMonoMass("E",false)-18.0105633;
        mod.label="Pyroglutamate";
      }
      break;
    case 'M':
      if(fabs(mass-147.0353937)<0.001 || fabs(mass-147.039885)<0.001) {
        mod.massDiffStd=15.9949141;
        mod.massStd=calcMonoMass("M",false)-15.9949141;
        mod.label="Oxidized_M";
      }
      break;
    case 'n':
      if(fabs(mass-43.0184)<0.001) {
        mod.massDiffStd=42.0105633;
        mod.massStd=0;
        mod.label="Acetylation";
      }
      break;
    case 'N':
      if(fabs(mass-115.0269)<0.001) {
        mod.massDiffStd=0.984016;
        mod.massStd=calcMonoMass("N",false)-0.984016;
        mod.label="Deamidation_N";
      }
      break;
    case 'Q':
      if(fabs(mass-129.0429)<0.001) {
        mod.massDiffStd=0.984016;
        mod.massStd=calcMonoMass("Q",false)-0.984016;
        mod.label="Deamidation_Q";
      }
      if(fabs(mass-111.0321)<0.001) {
        mod.massDiffStd=-17.026549;
        mod.massStd=calcMonoMass("Q",false)-17.026549;
        mod.label="Pyrrolidone";
      }
      break;
    case 'R':
      if(fabs(mass-157.0851)<0.001) {
        mod.massDiffStd=0.984016;
        mod.massStd=calcMonoMass("R",false)-0.984016;
        mod.label="Citrulline";
      }
      break;
    case 'S':
      if(fabs(mass-166.9984)<0.001) {
        mod.massDiffStd=79.9663289;
        mod.massStd=calcMonoMass("S",false)+79.9663289;
        mod.label="Phosphorylation_S";
      }
      break;
    case 'T':
      if(fabs(mass-181.0140)<0.001) {
        mod.massDiffStd=79.9663289;
        mod.massStd=calcMonoMass("T",false)+79.9663289;
        mod.label="Phosphorylation_T";
      }
      break;
    case 'Y':
      if(fabs(mass-243.0297)<0.001) {
        mod.massDiffStd=79.9663289;
        mod.massStd=calcMonoMass("Y",false)+79.9663289;
        mod.label="Phosphorylation_Y";
      }
      break;
    default:
      break;
  }

  if(mod.label.size()==0){
    cout << "Unknown modification: " << aa << " = " << mass << endl;
  }

  for(i=0;i<vMods.size();i++){
    if(vMods[i].aa==mod.aa && fabs(vMods[i].massSearch-mod.massSearch)<0.0000001) break;
  }
  if(i==vMods.size()){
    vMods.push_back(mod);
    return true;
  }
  return false;

}

double PepXMLParser3::calcMonoMass(char *seq, bool water){

	double mass=0.0;

	int H=0;
	int C=0;
	int N=0;
	int O=0;
	int S=0;

	unsigned int i;

	for(i=0;i<strlen(seq);i++){
		switch(seq[i]){
		case 'A':	C+= 3; H+= 5; N+= 1; O+= 1;	break;
		case 'R':	C+= 6; H+= 12; N+= 4;	O+= 1; break;
		case 'N': C+= 4; H+= 6; N+= 2; O+= 2;	break;    
		case 'D': C+= 4; H+= 5; N+= 1; O+= 3; break;
		case 'C': C+= 3; H+= 5;	N+= 1; O+= 1; S+= 1; break; //IAA treated samples
		case 'Q': C+= 5; H+= 8; N+= 2; O+= 2; break;
		case 'E': C+= 5; H+= 7; N+= 1; O+= 3; break;
		case 'G': C+= 2; H+= 3; N+= 1; O+= 1; break;
		case 'H': C+= 6; H+= 7; N+= 3; O+= 1; break;
		case 'I':
    case 'L':	C+= 6; H+= 11; N+= 1; O+= 1; break;
		case 'K': C+= 6; H+= 12; N+= 2; O+= 1; break;
    case 'M': C+= 5; H+= 9; N+= 1; O+= 1; S+= 1; break;
		case 'F': C+= 9; H+= 9; N+= 1; O+= 1; break;
		case 'P': C+= 5; H+= 7; N+= 1; O+= 1; break;
		case 'S': C+= 3; H+= 5; N+= 1; O+= 2; break;
		case 'T': C+= 4; H+= 7; N+= 1; O+= 2; break;
		case 'W': C+= 11; H+= 10; N+= 2; O+= 1; break;
		case 'Y': C+= 9; H+= 9; N+= 1; O+= 2; break;
		case 'V': C+= 5; H+= 9; N+= 1; O+= 1; break;
    default: break;
		}
	}

  if(water){
	  H+=2;
	  O++;
  }

	mass+=1.0078246*H;
	mass+=12.0000000*C;
	mass+=14.0030732*N;
	mass+=15.9949141*O;
	mass+=31.972070*S;

	return mass;
}

char PepXMLParser3::findMod(char aa, double mass){
  size_t i;
  for(i=0;i<vMods.size();i++){
    if(vMods[i].aa==aa && fabs(vMods[i].massSearch-mass)<0.0000001) return (char)i;
  }

  //didn't match mod, so add it
  if(!addMod(aa,mass)){
    cout << "WARNING: possible duplicate mod: " << aa << ", " << mass << endl;
  }
  return (char)i;
}

size_t PepXMLParser3::findProtein(string& s){
  size_t i;
  for(i=0;i<vProteins.size();i++){
    if(vProteins[i].compare(s)==0) return i;
  }
  vProteins.push_back(s);
  return i;
}

char PepXMLParser3::findScore(string& s){
  size_t i;
  for(i=0;i<vScores.size();i++){
    if(vScores[i].compare(s)==0) return (char)i;
  }
  vScores.push_back(s);
  return (char)i;
}

char PepXMLParser3::findXL(string& s, double mass){
  size_t i;
  for(i=0;i<vXL.size();i++){
    if(vXL[i].ID.compare(s)==0 && fabs(vXL[i].mass-mass)<0.0000001) return (char)i;
  }
  PepXMLXL p;
  p.ID=s;
  p.mass=mass;
  vXL.push_back(p);
  return (char)i;
}

string PepXMLParser3::getFile(int index){
  return vFiles[vSpectra[index].fileID];
}

int PepXMLParser3::getFileCount(){
  return (int)vFiles.size();
}

void PepXMLParser3::getFileFromList(int index, char* str){
  strcpy(str,&vFiles[index][0]);
}

PepXMLXL PepXMLParser3::getLinker(size_t index, char rank){
  size_t i;
  PepXMLXL xl;
  xl.ID.clear();
  xl.mass=0;
  if(index>=vSpectra.size()) return xl;
  for(i=0;i<vSpectra[index].size();i++){
    if(vSpectra[index][i].rank==rank) break;
  }
  if(i==vSpectra[index].size()) return xl;
  if(vSpectra[index][i].xlType==0) return xl;
  return vXL[vSpectra[index][i].xlIndex];
}

bool PepXMLParser3::getLinkSites(size_t index, char& a, char& b, char rank){
  size_t i,j,k;
  if(index>=vSpectra.size()) return false;
  for(i=0;i<vSpectra[index].size();i++){
    if(vSpectra[index][i].rank==rank) break;
  }
  if(i==vSpectra[index].size()) return false;
  switch(vSpectra[index][i].xlType) {
    case 1:
      k=0;
      for(j=0;j<vSpectra[index][i].peptide->xlScores->size();j++){
        if(getScoreLabel(vSpectra[index][i].peptide->xlScores->at(j).index).compare("link")==0){
          if(k==0) {
            a=(char)((int)(vSpectra[index][i].peptide->xlScores->at(j).value+0.5));
            k++;
          } else {
            b=(char)((int)(vSpectra[index][i].peptide->xlScores->at(j).value+0.5));
            break;
          }
        }
      }
      break;
    case 2:
      for(j=0;j<vSpectra[index][i].peptide->xlScores->size();j++){
        if(getScoreLabel(vSpectra[index][i].peptide->xlScores->at(j).index).compare("link")==0){
          a=(char)(vSpectra[index][i].peptide->xlScores->at(j).value+0.5);
          break;
        }
      }
      for(j=0;j<vSpectra[index][i].xlPeptide->xlScores->size();j++){
        if(getScoreLabel(vSpectra[index][i].xlPeptide->xlScores->at(j).index).compare("link")==0){
          b=(char)(vSpectra[index][i].xlPeptide->xlScores->at(j).value+0.5);
          break;
        }
      }
      break;
    default:
      return false;
  }
  return true;
}

char PepXMLParser3::getLinkType(size_t index, char rank){
  size_t i;
  if(index>=vSpectra.size()) return -1;
  for(i=0;i<vSpectra[index].size();i++){
    if(vSpectra[index][i].rank==rank) break;
  }
  if(i==vSpectra[index].size()) return -1;
  return vSpectra[index][i].xlType;
}

string PepXMLParser3::getPeptide(size_t index, bool mod, char rank, bool link){
  size_t i;
  if(index>=vSpectra.size()) return "";
  for(i=0;i<vSpectra[index].size();i++){
    if(vSpectra[index][i].rank==rank) break;
  }
  if(i==vSpectra[index].size()) return "";
  if(link){
    if(vSpectra[index][i].xlType<2) return "";
    if(mod){
      if(vSpectra[index][i].xlPeptide->modifiedPeptide.size()>0) return vSpectra[index][i].xlPeptide->modifiedPeptide;
    }
    return vSpectra[index][i].xlPeptide->peptide;
  }
  if(mod){
    if(vSpectra[index][i].peptide->modifiedPeptide.size()>0) return vSpectra[index][i].peptide->modifiedPeptide;
  }
  return vSpectra[index][i].peptide->peptide;
}

PepXMLMod PepXMLParser3::getPeptideMod(size_t pepIndex, size_t modIndex, char rank, bool link){
  size_t i;
  PepXMLMod p;
  p.aa=0;
  p.label.clear();
  p.massDiff=0;
  p.massDiffStd=0;
  p.massSearch=0;
  p.massStd=0;

  if(pepIndex>=vSpectra.size()) return p;
  for(i=0;i<vSpectra[pepIndex].size();i++){
    if(vSpectra[pepIndex][i].rank==rank) break;
  }
  if(i==vSpectra[pepIndex].size()) return p;
  if(link){
    if(vSpectra[pepIndex][i].xlType<2) return p;
    if(modIndex>=vSpectra[pepIndex][i].xlPeptide->mods->size()) return p;
    p=vMods[vSpectra[pepIndex][i].xlPeptide->mods->at(modIndex).index];
    p.aa=vSpectra[pepIndex][i].xlPeptide->mods->at(modIndex).pos;
    return p;
  }
  if(modIndex>=vSpectra[pepIndex][i].peptide->mods->size()) return p;
  p=vMods[vSpectra[pepIndex][i].peptide->mods->at(modIndex).index];
  p.aa=vSpectra[pepIndex][i].peptide->mods->at(modIndex).pos;
  return p;
}

size_t PepXMLParser3::getPeptideModCount(size_t index, char rank, bool link){
  size_t i;
  if(index>=vSpectra.size()) return 0;
  for(i=0;i<vSpectra[index].size();i++){
    if(vSpectra[index][i].rank==rank) break;
  }
  if(i==vSpectra[index].size()) return 0;
  if(link){
    if(vSpectra[index][i].xlType<2) return 0;
    return vSpectra[index][i].xlPeptide->mods->size();
  }
  return vSpectra[index][i].peptide->mods->size();
}

//bool PepXMLParser::getIprophet(){
//  return bIprophet;
//}

double PepXMLParser3::getProbability(double err){
  size_t i;
  for(i=0;i<vError.size();i++){
    if(vError[i].error==err) return vError[i].prob;
    if(vError[i].error>err) break;
  }
  //interpolate error
  double slope=(vError[i].error-vError[i-1].error)/(vError[i].prob-vError[i-1].prob);
  double intercept=vError[i].error-(vError[i].prob*slope);
  return (err-intercept)/slope;
}

string PepXMLParser3::getProtein(size_t index, size_t protIndex, char rank, bool link){
  size_t i;
  if(index>=vSpectra.size()) return "";
  for(i=0;i<vSpectra[index].size();i++){
    if(vSpectra[index][i].rank==rank) break;
  }
  if(i==vSpectra[index].size()) return "";
  if(link){
    if(vSpectra[index][i].xlType<2) return "";
    if(protIndex>=vSpectra[index][i].xlPeptide->proteins->size()) return "";
    return vProteins[vSpectra[index][i].xlPeptide->proteins->at(protIndex)];
  }
  if(protIndex>=vSpectra[index][i].peptide->proteins->size()) return "";
  return vProteins[vSpectra[index][i].peptide->proteins->at(protIndex)];
}

string PepXMLParser3::getScoreLabel(char scoreIndex){
  size_t i=(size_t)scoreIndex;
  if(i>=vScores.size()) return "";
  return vScores[i];
}

bool PepXMLParser3::hasIProphet(){
  return bIProphet;
}

bool PepXMLParser3::hasPepProphet(){
  return bPepProphet;
}

bool PepXMLParser3::readFile(const char* fileName) {

  XML_ParserFree(parser);
	parser = XML_ParserCreate(NULL);
	XML_SetUserData(parser, this);
	XML_SetElementHandler(parser, PepXMLParser3_startElementCallback, PepXMLParser3_endElementCallback);
	XML_SetCharacterDataHandler(parser, PepXMLParser3_charactersCallback);

	vSpectra.clear();
  vScores.clear();
  vProteins.clear();
  vMods.clear();
  vFiles.clear();
  vError.clear();
  vXL.clear();
  bIProphet=false;
  bPepProphet=false;
	
	FILE* fptr=fopen(fileName,"rt");
	if (fptr == NULL){
		cerr << "Error parse(): No open file." << endl;
		return false;
	}

	char buffer[16384];
	int readBytes = 0;
	bool success = true;
	int chunk=0;

	while (success && (readBytes = (int) fread(buffer, 1, sizeof(buffer), fptr)) != 0){
		success = (XML_Parse(parser, buffer, readBytes, false) != 0);
	}
	success = success && (XML_Parse(parser, buffer, 0, true) != 0);

	if (!success) {
		XML_Error error = XML_GetErrorCode(parser);

		cerr << fileName << "(" << XML_GetCurrentLineNumber(parser) << ") : error " << (int) error << ": ";
		switch (error) {
			case XML_ERROR_SYNTAX:
			case XML_ERROR_INVALID_TOKEN:
			case XML_ERROR_UNCLOSED_TOKEN:
				cerr << "Syntax error parsing XML.";
				break;
      case XML_ERROR_TAG_MISMATCH:
        cerr << "XML tag mismatch.";
        break;
      case XML_ERROR_DUPLICATE_ATTRIBUTE:
        cerr << "XML duplicate attribute.";
        break;
      case XML_ERROR_JUNK_AFTER_DOC_ELEMENT:
        cerr << "XML junk after doc element.";
        break;
			default:
				cerr << "XML Parsing error.";
				break;
		} 
		cerr << "\n";
		fclose(fptr);
		return false;
	}
	
	fclose(fptr);
	return true;
}

size_t PepXMLParser3::size(){
	return vSpectra.size();
}

/*
void PepXMLParser3::sortModPep(){
  qsort(&vPeptides[0],vPeptides.size(),sizeof(PepXMLEntry2),compareModPep);
}

void PepXMLParser3::sortScanNum(){
  qsort(&vPeptides[0],vPeptides.size(),sizeof(PepXMLEntry2),compareScanNum);
}

int PepXMLParser3::compareModPep(const void *p1, const void *p2){
  const PepXMLEntry2 d1 = *(PepXMLEntry2 *)p1;
  const PepXMLEntry2 d2 = *(PepXMLEntry2 *)p2;
  int i=d1.modifiedPeptide.compare(d2.modifiedPeptide);
  if(i<0) return -1;
  else if(i>0) return 1;
  else return 0;
}

int PepXMLParser2::compareScanNum(const void *p1, const void *p2){
  const PepXMLEntry2 d1 = *(PepXMLEntry2 *)p1;
  const PepXMLEntry2 d2 = *(PepXMLEntry2 *)p2;
  if(d1.scanNum<d2.scanNum) return -1;
  else if(d1.scanNum>d2.scanNum) return 1;
  else return 0;
}
*/
