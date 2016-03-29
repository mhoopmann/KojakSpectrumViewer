#include "CFragmentLists.h"

CFragmentLists::CFragmentLists(){

  int i,j;

  display=NULL;
  font=NULL;

  ionCount=0;
  aFrag=NULL;
  bFrag=NULL;
  cFrag=NULL;
  xFrag=NULL;
  yFrag=NULL;
  zFrag=NULL;

  aMatch=NULL;
  bMatch=NULL;
  cMatch=NULL;
  xMatch=NULL;
  yMatch=NULL;
  zMatch=NULL;

  for(i=0;i<3;i++){
    for(j=0;j<6;j++){
      bShowIons[i][j]=false;
    }
  }

  for(i=0;i<128;i++)  aaMass[i]=0;
  aaMass['A']=71.0371103;
  aaMass['C']=103.0091803;
  aaMass['D']=115.0269385;
  aaMass['E']=129.0425877;
  aaMass['F']=147.0684087;
  aaMass['G']=57.0214611;
  aaMass['H']=137.0589059;
  aaMass['I']=113.0840579;
  aaMass['K']=128.0949557;
  aaMass['L']=113.0840579;
  aaMass['M']=131.0404787;
  aaMass['N']=114.0429222;
  aaMass['P']=97.0527595;
  aaMass['Q']=128.0585714;
  aaMass['R']=156.1011021;
  aaMass['S']=87.0320244;
  aaMass['T']=101.0476736;
  aaMass['V']=99.0684087;
  aaMass['W']=186.0793065;
  aaMass['Y']=163.0633228;
}

CFragmentLists::~CFragmentLists(){
  display=NULL;
  font=NULL;
  deallocate();
}

void CFragmentLists::addMod(int pos, double modMass){
  double mMass=modMass-aaMass[peptide[pos]];
  for(int i=0;i<ionCount;i++){
    if(i>=pos){
      aFrag[0][i] += mMass;
      bFrag[0][i] += mMass;
      cFrag[0][i] += mMass;
      aFrag[1][i] += mMass/2;
      bFrag[1][i] += mMass/2;
      cFrag[1][i] += mMass/2;
      aFrag[2][i] += mMass/3;
      bFrag[2][i] += mMass/3;
      cFrag[2][i] += mMass/3;
    }
    if(i>=ionCount-pos){
      xFrag[0][i] += mMass;
      yFrag[0][i] += mMass;
      zFrag[0][i] += mMass;
      xFrag[1][i] += mMass/2;
      yFrag[1][i] += mMass/2;
      zFrag[1][i] += mMass/2;
      xFrag[2][i] += mMass/3;
      yFrag[2][i] += mMass/3;
      zFrag[2][i] += mMass/3;
    }
  }
}

void CFragmentLists::allocate(){
  size_t i;
  int j,k;

  deallocate();
  ionCount=peptide.size()-1;

  aMatch=new bool*[3];
  bMatch=new bool*[3];
  cMatch=new bool*[3];
  xMatch=new bool*[3];
  yMatch=new bool*[3];
  zMatch=new bool*[3];

  aFrag=new double*[3]; 
  bFrag=new double*[3];
  cFrag=new double*[3];
  xFrag=new double*[3];
  yFrag=new double*[3];
  zFrag=new double*[3];
  for(j=0;j<3;j++){
    aMatch[j]=new bool[ionCount];
    bMatch[j]=new bool[ionCount];
    cMatch[j]=new bool[ionCount];
    xMatch[j]=new bool[ionCount];
    yMatch[j]=new bool[ionCount];
    zMatch[j]=new bool[ionCount];

    aFrag[j]=new double[ionCount];
    bFrag[j]=new double[ionCount];
    cFrag[j]=new double[ionCount];
    xFrag[j]=new double[ionCount];
    yFrag[j]=new double[ionCount];
    zFrag[j]=new double[ionCount];
    for(k=0;k<ionCount;k++){
      aMatch[j][k]=false;
      bMatch[j][k]=false;
      cMatch[j][k]=false;
      xMatch[j][k]=false;
      yMatch[j][k]=false;
      zMatch[j][k]=false;

      aFrag[j][k]=0;
      bFrag[j][k]=0;
      cFrag[j][k]=0;
      xFrag[j][k]=0;
      yFrag[j][k]=0;
      zFrag[j][k]=0;
    }
  }

  peptideMass=18.0105633;
  for(i=0;i<peptide.size();i++) peptideMass+=aaMass[peptide[i]];
}


void CFragmentLists::calcIons(int link, int link2, double modMass){

  int b;
	int i;
  int j;
  int y;
	double mMass;
  char str[32];

  //set up boundaries
  int ionCount=peptide.size()-1; //pep1Len-1;
  b=0;
  y=ionCount-1;

	//b- & y-ions from first peptide
  mMass=0;
	for(i=0;i<ionCount;i++){
    mMass+=aaMass[peptide[i]];
    if(link>-1 && i>=link) {

      if(link2>-1 && i<link2) {

        xFrag[0][y] = -9000;
        yFrag[0][y] = -9000;
        zFrag[0][y] = -9000;
        xFrag[1][y] = -9000;
        yFrag[1][y] = -9000;
        zFrag[1][y] = -9000;
        xFrag[2][y] = -9000;
        yFrag[2][y] = -9000;
        zFrag[2][y] = -9000;

        aFrag[0][b] = -9000;
        bFrag[0][b] = -9000;
        cFrag[0][b] = -9000;
        aFrag[1][b] = -9000;
        bFrag[1][b] = -9000;
        cFrag[1][b] = -9000;
        aFrag[2][b] = -9000;
        bFrag[2][b] = -9000;
        cFrag[2][b] = -9000;

      } else {

        xFrag[0][y] = peptideMass - modMass - mMass + 25.9792649 + 1.007276466;
        yFrag[0][y] = peptideMass - modMass - mMass + 1.007276466;
        zFrag[0][y] = peptideMass - modMass - mMass - 16.0187224 + 1.007276466;   //z.
        xFrag[1][y] = (xFrag[0][y] + 1.007276466)/2;
        yFrag[1][y] = (yFrag[0][y] + 1.007276466)/2;
        zFrag[1][y] = (zFrag[0][y] + 1.007276466)/2;
        xFrag[2][y] = (xFrag[0][y] + 2.014552932)/3;
        yFrag[2][y] = (yFrag[0][y] + 2.014552932)/3;
        zFrag[2][y] = (zFrag[0][y] + 2.014552932)/3;

        aFrag[0][b] = mMass + modMass - 27.9949141 + 1.007276466;
        bFrag[0][b] = mMass + modMass + 1.007276466;
        cFrag[0][b] = mMass + modMass + 17.026547 + 1.007276466;
        aFrag[1][b] = (aFrag[0][b] + 1.007276466)/2;
        bFrag[1][b] = (bFrag[0][b] + 1.007276466)/2;
        cFrag[1][b] = (cFrag[0][b] + 1.007276466)/2;
        aFrag[2][b] = (aFrag[0][b] + 2.014552932)/3;
        bFrag[2][b] = (bFrag[0][b] + 2.014552932)/3;
        cFrag[2][b] = (cFrag[0][b] + 2.014552932)/3;

      }

    } else {

      xFrag[0][y] = peptideMass - mMass + 25.9792649 + 1.007276466;
      yFrag[0][y] = peptideMass - mMass + 1.007276466;
      zFrag[0][y] = peptideMass - mMass - 16.0187224 + 1.007276466;
      xFrag[1][y] = (xFrag[0][y] + 1.007276466)/2;
      yFrag[1][y] = (yFrag[0][y] + 1.007276466)/2;
      zFrag[1][y] = (zFrag[0][y] + 1.007276466)/2;
      xFrag[2][y] = (xFrag[0][y] + 2.014552932)/3;
      yFrag[2][y] = (yFrag[0][y] + 2.014552932)/3;
      zFrag[2][y] = (zFrag[0][y] + 2.014552932)/3;

      aFrag[0][b] = mMass - 27.9949141 + 1.007276466;
      bFrag[0][b] = mMass + 1.007276466;
      cFrag[0][b] = mMass + 17.026547 + 1.007276466;
      aFrag[1][b] = (aFrag[0][b] + 1.007276466)/2;
      bFrag[1][b] = (bFrag[0][b] + 1.007276466)/2;
      cFrag[1][b] = (cFrag[0][b] + 1.007276466)/2;
      aFrag[2][b] = (aFrag[0][b] + 2.014552932)/3;
      bFrag[2][b] = (bFrag[0][b] + 2.014552932)/3;
      cFrag[2][b] = (cFrag[0][b] + 2.014552932)/3;

    }
    y--;
    b++;
	}
}

void CFragmentLists::deallocate(){
  int j;

  if(aMatch!=NULL){
    for(j=0;j<3;j++) delete [] aMatch[j];
    delete [] aMatch;
    aMatch=NULL;
  }
  if(bMatch!=NULL){
    for(j=0;j<3;j++) delete [] bMatch[j];
    delete [] bMatch;
    bMatch=NULL;
  }
  if(cMatch!=NULL){
    for(j=0;j<3;j++) delete [] cMatch[j];
    delete [] cMatch;
    cMatch=NULL;
  }
  if(xMatch!=NULL){
    for(j=0;j<3;j++) delete [] xMatch[j];
    delete [] xMatch;
    xMatch=NULL;
  }
  if(yMatch!=NULL){
    for(j=0;j<3;j++) delete [] yMatch[j];
    delete [] yMatch;
    yMatch=NULL;
  }
  if(zMatch!=NULL){
    for(j=0;j<3;j++) delete [] zMatch[j];
    delete [] zMatch;
    zMatch=NULL;
  }

  if(aFrag!=NULL){
    for(j=0;j<3;j++) delete [] aFrag[j];
    delete [] aFrag;
    aFrag=NULL;
  }
  if(bFrag!=NULL){
    for(j=0;j<3;j++) delete [] bFrag[j];
    delete [] bFrag;
    bFrag=NULL;
  }
  if(cFrag!=NULL){
    for(j=0;j<3;j++) delete [] cFrag[j];
    delete [] cFrag;
    cFrag=NULL;
  }
  if(xFrag!=NULL){
    for(j=0;j<3;j++) delete [] xFrag[j];
    delete [] xFrag;
    xFrag=NULL;
  }
  if(yFrag!=NULL){
    for(j=0;j<3;j++) delete [] yFrag[j];
    delete [] yFrag;
    yFrag=NULL;
  }
  if(zFrag!=NULL){
    for(j=0;j<3;j++) delete [] zFrag[j];
    delete [] zFrag;
    zFrag=NULL;
  }
}

double  CFragmentLists::getAIon(int ch, int index){
  return aFrag[ch-1][index];
}
double  CFragmentLists::getBIon(int ch, int index){
  return bFrag[ch-1][index];
}
double  CFragmentLists::getCIon(int ch, int index){
  return cFrag[ch-1][index];
}
double  CFragmentLists::getXIon(int ch, int index){
  return xFrag[ch-1][index];
}
double  CFragmentLists::getYIon(int ch, int index){
  return yFrag[ch-1][index];
}
double  CFragmentLists::getZIon(int ch, int index){
  return zFrag[ch-1][index];
}

void CFragmentLists::init(){
  /*
  bIon[0].posX=0;
  bIon[0].posY=0;
  bIon[0].szX=64;
  bIon[0].szY=300;
  bIon[0].szFont=12;
  bIon[0].cellHeight=14;
  bIon[0].cellWidth=64;

  numList.posX=64;
  numList.posY=0;
  numList.szFont=12;
  numList.szX=24;
  numList.szY=300;
  numList.cellHeight=14;
  numList.cellWidth=24;

  yIon[0].posX=88;
  yIon[0].posY=0;
  yIon[0].szX=64;
  yIon[0].szY=300;
  yIon[0].szFont=12;
  yIon[0].cellHeight=14;
  yIon[0].cellWidth=64;
  */
}

bool CFragmentLists::logic(){

  return true;
}

bool CFragmentLists::render(){
  SDL_Rect r;
  char str[12];
  char str2[12];
  int i,j,k,n,q;

  //set the font size
  int fontSize=font->fontSize;
  font->fontSize=12;

  //print table headers
  n=2;
  for(j=0;j<6;j++){
    for(k=0;k<3;k++){
      if(k==0)strcpy(str,"+");
      else sprintf(str,"%d+",k+1);
      if(bShowIons[k][j]) {
        r.x=posX+n;
        r.y=posY+5;
        r.w=56;
        r.h=15;
        SDL_SetRenderDrawColor(display->renderer,58,58,58,255);
        SDL_RenderFillRect(display->renderer,&r);
        SDL_SetRenderDrawColor(display->renderer,234,234,234,255);
        SDL_RenderDrawRect(display->renderer,&r);
        switch(j){
          case 0: sprintf(str2,"a%s",str); break;
          case 1: sprintf(str2,"b%s",str); break;
          case 2: sprintf(str2,"c%s",str); break;
          case 3: sprintf(str2,"x%s",str); break;
          case 4: sprintf(str2,"y%s",str); break;
          case 5: sprintf(str2,"z%s",str); break;
          default:
            break;
        }
        i=font->getStringWidth(str2);
        font->render(posX+n+28-i/2,posY+5,str2,0);
        n+=56;
      }
    }
    if(j==2) {
      r.x=posX+n; 
      r.y=posY+5; 
      r.w=14; 
      r.h=15;
      SDL_SetRenderDrawColor(display->renderer,58,58,58,255);
      SDL_RenderFillRect(display->renderer,&r);
      SDL_SetRenderDrawColor(display->renderer,234,234,234,255);
      SDL_RenderDrawRect(display->renderer,&r);
      font->render(posX+n+3,posY+5,"#",0);
      n+=14;
      
      r.x=posX+n; 
      SDL_SetRenderDrawColor(display->renderer,58,58,58,255);
      SDL_RenderFillRect(display->renderer,&r);
      SDL_SetRenderDrawColor(display->renderer,234,234,234,255);
      SDL_RenderDrawRect(display->renderer,&r);
      n+=14;

      r.x=posX+n; 
      SDL_SetRenderDrawColor(display->renderer,58,58,58,255);
      SDL_RenderFillRect(display->renderer,&r);
      SDL_SetRenderDrawColor(display->renderer,234,234,234,255);
      SDL_RenderDrawRect(display->renderer,&r);
      font->render(posX+n+3,posY+5,"#",0);
      n+=14;
    }
  }

  for(i=0;i<ionCount+1;i++){
    n=2;
    for(j=0;j<6;j++){
      for(k=0;k<3;k++){
        if(bShowIons[k][j]) {
          if(j>2 && i==0) {
            n+=56;
            continue;
          }
          if(j<3 && i>=ionCount) {
            n+=56;
            continue;
          }
          switch(j){
            case 0: 
              if(aFrag[k][i]<0) strcpy(str,"---");
              else sprintf(str,"%.3lf",aFrag[k][i]); 
              break;
            case 1: 
              if(bFrag[k][i]<0) strcpy(str,"---");
              else sprintf(str,"%.3lf",bFrag[k][i]); 
              break;
            case 2: 
              if(cFrag[k][i]<0) strcpy(str,"---");
              else sprintf(str,"%.3lf",cFrag[k][i]); 
              break;
            case 3: 
              if(xFrag[k][ionCount-i]<0) strcpy(str,"---");
              else sprintf(str,"%.3lf",xFrag[k][ionCount-i]); 
              break;
            case 4: 
              if(yFrag[k][ionCount-i]<0) strcpy(str,"---");
              else sprintf(str,"%.3lf",yFrag[k][ionCount-i]); 
              break;
            case 5: 
              if(zFrag[k][ionCount-i]<0) strcpy(str,"---");
              else sprintf(str,"%.3lf",zFrag[k][ionCount-i]); 
              break;
            default:
              break;
          }
          r.x=posX+n;
          r.y=posY+i*12+20;
          r.w=56;
          r.h=12;
          SDL_SetRenderDrawColor(display->renderer,234,234,234,255);
          switch(j){
            case 0: if(aMatch[k][i]) SDL_SetRenderDrawColor(display->renderer,colorIons[k][j].r,colorIons[k][j].g,colorIons[k][j].b,255); break;
            case 1: if(bMatch[k][i]) SDL_SetRenderDrawColor(display->renderer,colorIons[k][j].r,colorIons[k][j].g,colorIons[k][j].b,255); break;
            case 2: if(cMatch[k][i]) SDL_SetRenderDrawColor(display->renderer,colorIons[k][j].r,colorIons[k][j].g,colorIons[k][j].b,255); break;
            case 3: if(xMatch[k][ionCount-i]) SDL_SetRenderDrawColor(display->renderer,colorIons[k][j].r,colorIons[k][j].g,colorIons[k][j].b,255); break;
            case 4: if(yMatch[k][ionCount-i]) SDL_SetRenderDrawColor(display->renderer,colorIons[k][j].r,colorIons[k][j].g,colorIons[k][j].b,255); break;
            case 5: if(zMatch[k][ionCount-i]) SDL_SetRenderDrawColor(display->renderer,colorIons[k][j].r,colorIons[k][j].g,colorIons[k][j].b,255); break;
            default:
              break;
          }
          SDL_RenderFillRect(display->renderer,&r);
          q=font->getStringWidth(str);
          font->render(posX+n+28-q/2,posY+i*12+20,str,1);
          n+=56;
        }
      }
      if(j==2) {
        r.x=posX+n; 
        r.y=posY+i*12+20; 
        r.w=14; 
        r.h=12;
        SDL_SetRenderDrawColor(display->renderer,234,234,234,255);
        SDL_RenderFillRect(display->renderer,&r);
        sprintf(str,"%d",i+1);
        q=font->getStringWidth(str);
        font->render(posX+n+7-q/2,posY+i*12+20,str,1);
        n+=14;

        r.x=posX+n; 
        SDL_RenderFillRect(display->renderer,&r);
        sprintf(str,"%c",peptide[i]);
        q=font->getStringWidth(str);
        font->render(posX+n+7-q/2,posY+i*12+20,str,1);
        n+=14;

        r.x=posX+n; 
        SDL_RenderFillRect(display->renderer,&r);
        sprintf(str,"%d",ionCount-i+1);
        q=font->getStringWidth(str);
        font->render(posX+n+7-q/2,posY+i*12+20,str,1);
        n+=14;
      }
    }
  }

  font->fontSize=fontSize;
  return true;
}

void CFragmentLists::setDisplay(CDisplay* d){
  display=d;
  size_t i,j;
  for(i=0;i<3;i++) {
    for(j=0;j<6;j++) colorIons[i][j]=display->pal.spectrumIons[i][j];
  }
}

void CFragmentLists::setFont(CFont* f){
  font=f;
  //numList.setFont(f);
  //bIon[0].setFont(f);
  //yIon[0].setFont(f);
}

void CFragmentLists::setMatch(int ion, int ch, int index, bool value){
  switch(ion){
    case 0: aMatch[ch][index]=value; break;
    case 1: bMatch[ch][index]=value; break;
    case 2: cMatch[ch][index]=value; break;
    case 3: xMatch[ch][index]=value; break;
    case 4: yMatch[ch][index]=value; break;
    case 5: zMatch[ch][index]=value; break;
    default:
      break;
  }
}

void CFragmentLists::setPeptide(string pep, int ch, int linkPos, int linkPos2, double linkMass){
  peptide=pep;
  charge=ch;
  allocate();
  peptideMass+=linkMass;
  calcIons(linkPos,linkPos2,linkMass);
}

int CFragmentLists::size(){
  return ionCount;
}
