#include "CSpectrumGraph.h"

CSpectrumGraph::CSpectrumGraph(){
  font=NULL;
  display=NULL;
  input=NULL;
  szX=128;
  szY=128;
  posX=0;
  posY=0;
  lowX=100;
  highX=1000;
  lowY=0;
  highY=1000;
  zoomLock=false;
}

CSpectrumGraph::CSpectrumGraph(CDisplay* d, CInput* inp){
  font=NULL;
  display=d;
  input=inp;
  szX=128;
  szY=128;
  posX=0;
  posY=0;
  lowX=100;
  highX=1000;
  lowY=0;
  highY=1000;
  zoomLock=false;
}

CSpectrumGraph::~CSpectrumGraph(){
  display=NULL;
  input=NULL;
}

bool CSpectrumGraph::logic(int mouseX, int mouseY, int mouseButton, bool mouseButton1){

  //check local item logic
  if(mouseButton1 && mouseX>=posX+10 && mouseX<=posX+szX-10 && mouseY>=posY && mouseY<=posY+szY){
    if(zoomLock){
      if(mouseButton1){
        lockPos2=mouseX;
      } else {
        zoomLock=false;
      }
    } else {
      zoomLock=true;
      lockPos1=mouseX;
      lockPos2=mouseX;
    }
    return true;
  } else {
    if(zoomLock){
      if(mouseButton1) return true;
      if(lockPos1==lockPos2) {
        zoomLock=false;
        return true;
      }
      if(lockPos2<lockPos1){
        int i=lockPos1;
        lockPos1=lockPos2;
        lockPos2=i;
      }
      double ppuX,ppuY,d;
      ppuX = (double)(highX-lowX)/(szX-20);
      d=lowX;
      lowX = (lockPos1-10)*ppuX+d;
      highX = (lockPos2-lockPos1)*ppuX+lowX;
      highY=0;
      for(size_t j=0;j<spectrum.size();j++){
        if(spectrum[j].x>=lowX && spectrum[j].x<=highX && spectrum[j].y>highY) highY=spectrum[j].y;
      }
      zoomLock=false;
    }
  }
  if(mouseButton==2){
    resetView();
  }
  return false;
}

bool CSpectrumGraph::render(CPeptideBox& p){
  SDL_Rect r;
  size_t i,j;
  int k;
  int x,y;
  int x2,y2,m;
  double ppuX,ppuY;  //pixels per unit
  char str[32];
  string st;
  CFragmentLists* f;
  int fontSize=font->fontSize;

  //Render background
  r.x=posX;
  r.y=posY;
  r.w=szX;
  r.h=szY;
  SDL_SetRenderDrawColor(display->renderer,color[0].r,color[0].g,color[0].b,255);
  SDL_RenderFillRect(display->renderer,&r);

  //Calculate scale to fit window
  ppuX = (double)(szX-20)/(highX-lowX);
  if(highY>0) ppuY = (double)(szY-60)/highY;
  else ppuY=0;

  //Draw spectrum
  SDL_SetRenderDrawColor(display->renderer,color[1].r,color[1].g,color[1].b,255);
  y=posY+szY-10;
  switch(spectrum.type){
    case grBar:
      for(i=0;i<spectrum.size();i++) {
        if(spectrum[i].x<lowX || spectrum[i].x>highX) continue;
        x=posX+(int)((spectrum[i].x-lowX)*ppuX)+10;
        SDL_RenderDrawLine(display->renderer,x,y,x,y-(int)(spectrum[i].y*ppuY));
      }
      break;
    case grLine:
      x2=0;
      y2=posY+szY-10;
      m=posY+szY-10;
      for(i=0;i<spectrum.size();i++) {
        x=posX+(int)((spectrum[i].x-lowX)*ppuX)+10;
        y=m-(int)(spectrum[i].y*ppuY);
        if(spectrum[i].x<lowX || spectrum[i].x>highX) {
          x2=x;
          y2=y;
          continue;
        }
        SDL_RenderDrawLine(display->renderer,x2,y2,x,y);
        x2=x;
        y2=y;
      }
      break;
    case grPoint:
    default:
      break;
  }

  //Color fragment ions, peptide A
  font->fontSize=10;
  y=posY+szY-10;
  if(p.showPeptide(true)){
    f=p.getIonSeries();
    for(i=0;i<6;i++){
      for(j=0;j<3;j++){
        if(!p.checkSeries(j,i)) continue;
        for(k=0;k<f->size();k++){
          if(i==0) m=spectrum.findMass(f->getAIon(j+1,k));
          else if(i==1) m=spectrum.findMass(f->getBIon(j+1,k));
          else if(i==2) m=spectrum.findMass(f->getCIon(j+1,k));
          else if(i==3) m=spectrum.findMass(f->getXIon(j+1,k));
          else if(i==4) m=spectrum.findMass(f->getYIon(j+1,k));
          else if(i==5) m=spectrum.findMass(f->getZIon(j+1,k));
          if(m<0) {
            f->setMatch(i,j,k,false);
            continue;
          }
          f->setMatch(i,j,k,true);
          if(spectrum[m].x<lowX || spectrum[m].x>highX) continue;
          x=posX+(int)((spectrum[m].x-lowX)*ppuX)+10;
          SDL_SetRenderDrawColor(display->renderer,colorIons[j][i].r,colorIons[j][i].g,colorIons[j][i].b,255);
          SDL_RenderDrawLine(display->renderer,x,y,x,y-(int)(spectrum[m].y*ppuY));
          if(i==0) st="a";
          else if(i==1) st="b";
          else if(i==2) st="c";
          else if(i==3) st="x";
          else if(i==4) st="y";
          else if(i==5) st="z";
          sprintf(str,"%d",k+1);
          st+=str;
          if(j==0) st+="+";
          else if(j==1) st+="++";
          else if(j==2) st+="+++";
          if(p.getIonSeries(true)!=NULL) st+=", alpha";
          font->render(x-5,y-(int)(spectrum[m].y*ppuY)-5,st,1,true);
        }
      }
    }
  }

  //Color fragment ions, peptide B
  f=p.getIonSeries(true);
  if(f!=NULL && p.showPeptide(false)){
    for(i=0;i<6;i++){
      for(j=0;j<3;j++){
        if(!p.checkSeries(j,i)) continue;
        for(k=0;k<f->size();k++){
          if(i==0) m=spectrum.findMass(f->getAIon(j+1,k));
          else if(i==1) m=spectrum.findMass(f->getBIon(j+1,k));
          else if(i==2) m=spectrum.findMass(f->getCIon(j+1,k));
          else if(i==3) m=spectrum.findMass(f->getXIon(j+1,k));
          else if(i==4) m=spectrum.findMass(f->getYIon(j+1,k));
          else if(i==5) m=spectrum.findMass(f->getZIon(j+1,k));
          if(m<0) {
            f->setMatch(i,j,k,false);
            continue;
          }
          f->setMatch(i,j,k,true);
          if(spectrum[m].x<lowX || spectrum[m].x>highX) continue;
          x=posX+(int)((spectrum[m].x-lowX)*ppuX)+10;
          SDL_SetRenderDrawColor(display->renderer,colorIons[j][i].r,colorIons[j][i].g,colorIons[j][i].b,255);
          SDL_RenderDrawLine(display->renderer,x,y,x,y-(int)(spectrum[m].y*ppuY));
          if(i==0) st="a";
          else if(i==1) st="b";
          else if(i==2) st="c";
          else if(i==3) st="x";
          else if(i==4) st="y";
          else if(i==5) st="z";
          sprintf(str,"%d",k+1);
          st+=str;
          if(j==0) st+="+";
          else if(j==1) st+="++";
          else if(j==2) st+="+++";
          st+=", beta";
          font->render(x-5,y-(int)(spectrum[m].y*ppuY)-5,st,1,true);
        }
      }
    }
  }

  //Draw x-axis
  if(posX+szX-10>posX+10){
    SDL_SetRenderDrawColor(display->renderer,color[2].r,color[2].g,color[2].b,255);
    SDL_RenderDrawLine(display->renderer,posX+10,posY+szY-10,posX+szX-10,posY+szY-10);
    font->setFontSize(8);
    sprintf(str,"%.2lf",lowX);
    font->render(10,posY+szY-9,str,1);
    sprintf(str,"%.2lf",highX);
    font->render(posX+szX-30,posY+szY-9,str,1);
    for(i=szX/5+posX+10;i<szX+posX-20;i+=szX/5){
      sprintf(str,"%.2lf",(double)i/ppuX+lowX);
      font->render(i+10,posY+szY-9,str,1);
    }
  }

  //Draw zoom window
  if(zoomLock){
    r.x=lockPos1;
    r.w=lockPos2-lockPos1;
    r.y=posY+10;
    r.h=szY-20;
    SDL_SetRenderDrawColor(display->renderer,0,0,0,128);
    SDL_RenderFillRect(display->renderer,&r);
    
    SDL_SetRenderDrawColor(display->renderer,0,0,0,255);
    SDL_RenderDrawLine(display->renderer,lockPos1,posY+10,lockPos1,posY+szY-10);
    SDL_RenderDrawLine(display->renderer,lockPos2,posY+10,lockPos2,posY+szY-10);
  }

  font->fontSize=fontSize;
  f=NULL;
  return true;
}

void CSpectrumGraph::resetView(){
  lowX=spectrum.getMinX()-10;
  if(lowX<0) lowX=0;
  highX=spectrum.getMaxX()+10;
  lowY=0;
  highY=spectrum.getMaxY();
}

void CSpectrumGraph::setDisplay(CDisplay *d){
  display=d;
  size_t i,j;
  for(i=0;i<3;i++) {
    color[i]=display->pal.spectrum[i];
    for(j=0;j<6;j++) colorIons[i][j]=display->pal.spectrumIons[i][j];
  }
}

void CSpectrumGraph::setFont(CFont* f){
  font=f;
}

void CSpectrumGraph::setInput(CInput *inp){
  input=inp;
}



