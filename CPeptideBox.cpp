#include "CPeptideBox.h"

CPeptideBox::CPeptideBox(){
  activeFocus = NULL;
  display = NULL;
  font = NULL;
  
  posX=0;
  posY=0;
  szX=0;
  szY=0;

  charge=0;
  scanNumber=0;
  clear=false;
  mass=0;

  cbPepA.posX=4;
  cbPepA.posY=20;
  cbPepA.sz=12;
  cbPepA.caption="PEPTIDEAR";
  cbPepA.checked=true;

  cbPepB.posX=4;
  cbPepB.posY=40;
  cbPepB.sz=12;
  cbPepB.caption="SOMEOTHERPEPK";

  for(int i=0;i<3;i++){
    for(int j=0;j<6;j++){
      cbIons[i][j].posX=j*16+16;
      cbIons[i][j].posY=i*16+69;
      cbIons[i][j].sz=12;
    }
  }
  cbIons[0][1].checked=true;
  cbIons[0][4].checked=true;

  fragPepA.posX=4;
  fragPepA.posY=113;

  fragPepB.posX=4;
  fragPepB.posY=113;

  showScrollbarV=false;
  scrollLockV=false;
  scrollOffsetV=0;

  showScrollbarH=false;
  scrollLockH=false;
  scrollOffsetH=0;

}

CPeptideBox::~CPeptideBox(){
  activeFocus = NULL;
  display = NULL;
  font = NULL;
}

bool CPeptideBox::checkSeries (int charge, int ion){
  return cbIons[charge][ion].checked;
}

void CPeptideBox::fixLayout(){
  int w,h;
  int x,y;
  int contentSize;
  int scrollTrackSpace;
  int scrollThumbSpace;
  int viewSize;

  display->getWindowSize(w,h);  
  
  //Vertical Scrolling
  viewSize = h-157;
  contentSize = 0; //size of visible fragment lists
  if(cbPepA.checked) contentSize+=(fragPepA.size()+1)*14+23; 
  if(cbPepB.active && cbPepB.checked) contentSize+=(fragPepB.size()+1)*14+23;
  if(viewSize>0 && contentSize>viewSize) {
    showScrollbarV=true;
    
    thumbHeightV = viewSize*viewSize/contentSize;
    thumbMaxV = viewSize-thumbHeightV;

    scrollTrackSpace = contentSize-viewSize;
    scrollThumbSpace = viewSize-thumbHeightV;
    scrollJumpV = (double)scrollTrackSpace/scrollThumbSpace;
    scrollOffsetV = 0;
  } else showScrollbarV=false;

  //Horizontal Scrolling
  viewSize = szX-14;
  contentSize = 54; //size of visible fragment lists
  for(x=0;x<3;x++){
    for(y=0;y<6;y++){
      if(cbIons[x][y].checked) contentSize+=56;
    }
  }
  if(viewSize>0 && contentSize>viewSize) {
    showScrollbarH=true;
    
    thumbHeightH = viewSize*viewSize/contentSize;
    thumbMaxH = viewSize-thumbHeightH;

    scrollTrackSpace = contentSize-viewSize;
    scrollThumbSpace = viewSize-thumbHeightH;
    scrollJumpH = (double)scrollTrackSpace/scrollThumbSpace;
    scrollOffsetH = 0;
  } else showScrollbarH=false;

}

CFragmentLists* CPeptideBox::getIonSeries(bool pepB){
  if(pepB) {
    if(cbPepB.active==false) return NULL;
    return &fragPepB;
  }
  return &fragPepA;
}

int CPeptideBox::logic(int mouseX, int mouseY, int mouseButton, bool mouseButton1){
  int i,j;

  if(clear) return 0;

  //this object is in a viewport. Check logic relative to the viewport
  mouseX-=posX;
  mouseY-=posY;

  if(mouseButton1 && showScrollbarV){
    //grabbed scrollbar
    if(mouseX>=szX-8 && mouseX<=szX-2 && mouseY>=115+scrollOffsetV && mouseY<=115+scrollOffsetV+thumbHeightV){
      if(scrollLockV){
        //printf("Scrolled: %d %d\n",mouseY, lastMouseY);
        scrollOffsetV+=(mouseY-lastMouseY);
        if(scrollOffsetV>thumbMaxV) scrollOffsetV=thumbMaxV;
        if(scrollOffsetV<0) scrollOffsetV=0;
        lastMouseY=mouseY;
      } else {
        scrollLockV=true;
        lastMouseY=mouseY;
        //printf("Locking %d\n",lastMouseY);
      }
      return 1;
    } else if(scrollLockV){
      //printf("Scrolled: %d %d\n",mouseY, lastMouseY);
      scrollOffsetV+=(mouseY-lastMouseY);
      if(scrollOffsetV>thumbMaxV) scrollOffsetV=thumbMaxV;
      if(scrollOffsetV<0) scrollOffsetV=0;
      lastMouseY=mouseY;
      return 1;
    }
  } 

  if(mouseButton==1 && scrollLockV){
    scrollLockV=false;
    return 1;
  }

  if(mouseButton1 && showScrollbarH){
    //grabbed scrollbar
    if(mouseX>=scrollOffsetH && mouseX<=scrollOffsetH+thumbHeightH && mouseY>=szY-8 && mouseY<=szY-2){
      if(scrollLockH){
        //printf("Scrolled: %d %d\n",mouseY, lastMouseY);
        scrollOffsetH+=(mouseX-lastMouseX);
        if(scrollOffsetH>thumbMaxH) scrollOffsetH=thumbMaxH;
        if(scrollOffsetH<0) scrollOffsetH=0;
        lastMouseX=mouseX;
      } else {
        scrollLockH=true;
        lastMouseX=mouseX;
        //printf("Locking %d\n",lastMouseY);
      }
      return 1;
    } else if(scrollLockH){
      //printf("Scrolled: %d %d\n",mouseY, lastMouseY);
      scrollOffsetH+=(mouseX-lastMouseX);
      if(scrollOffsetH>thumbMaxH) scrollOffsetH=thumbMaxH;
      if(scrollOffsetH<0) scrollOffsetH=0;
      lastMouseX=mouseX;
      return 1;
    }
  } 

  if(mouseButton==1 && scrollLockH){
    scrollLockH=false;
    return 1;
  }

  if(cbPepA.logic(mouseX,mouseY,mouseButton)) {
    fixLayout();
    return 1;
  }

  if(cbPepB.active){
    if(cbPepB.logic(mouseX,mouseY,mouseButton)) {
      fixLayout();
      return 1;
    }
  }

  for(i=0;i<3;i++){
    for(j=0;j<6;j++){
      if(cbIons[i][j].logic(mouseX,mouseY,mouseButton)){
        fragPepA.bShowIons[i][j]=cbIons[i][j].checked;
        fragPepB.bShowIons[i][j]=cbIons[i][j].checked;
        fixLayout();
        return 1;
      }
    }
  }

  scrollLockH=false;
  scrollLockV=false;
  return 0; 
}

void CPeptideBox::render(int x, int y){
  SDL_Rect origVP;
  SDL_Rect vp;
  SDL_Rect r;
  char str[64];
  int fontSize=font->fontSize;

  SDL_RenderGetViewport(display->renderer,&origVP);

  //Render from here to the end of the window;
  vp=origVP;
  vp.x=posX;
  vp.y=posY;
  vp.w=origVP.w-posX;
  vp.h=origVP.h-posY;
  SDL_RenderSetViewport(display->renderer,&vp);

  r=vp;
  r.x=0;
  r.y=0;
  SDL_SetRenderDrawColor(display->renderer,255,255,255,255);
  SDL_RenderFillRect(display->renderer,&r);

  //if there is nothing to display, return now
  if(clear) {
    SDL_RenderSetViewport(display->renderer, &origVP);
    font->fontSize=fontSize;
    return;
  }

  //Render everything in the viewport
  //Render the scan information
  sprintf(str,"Scan: %d,  Mass: %.4lf Da,  Charge: %d+", scanNumber,mass, charge);
  font->fontSize=12;
  font->render(4,4,str,1);

  //Render peptide checkboxes
  cbPepA.render();
  if(cbPepB.active) cbPepB.render();

  //Render link-lines
  if(rLinkA.x>-1){
    SDL_SetRenderDrawColor(display->renderer,32,32,32,255);
    SDL_RenderDrawLine(display->renderer,rLinkA.x,rLinkA.y,rLinkA.x,rLinkA.y+rLinkA.h);
    SDL_RenderDrawLine(display->renderer,rLinkB.x,rLinkB.y,rLinkB.x,rLinkB.y+rLinkB.h);
    SDL_RenderDrawLine(display->renderer,rLinkA.x,rLinkA.y+rLinkA.h,rLinkB.x,rLinkB.y);
  }

  font->fontSize=10;
  font->render(4,69,"1+",1);
  font->render(4,85,"2+",1);
  font->render(4,101,"3+",1);
  font->render(20,55,"a",1);
  font->render(36,55,"b",1);
  font->render(52,55,"c",1);
  font->render(68,55,"x",1);
  font->render(84,55,"y",1);
  font->render(100,55,"z",1);
  for(int i=0;i<3;i++){
    for(int j=0;j<6;j++){
      cbIons[i][j].render();
    }
  }

  //New viewport for scrollable content
  //also excludes area that scroll bars will occupy
  vp.x=posX+4;
  vp.y=posY+115;
  vp.w=origVP.w-posX-14;
  vp.h=origVP.h-posY-125;
  SDL_RenderSetViewport(display->renderer,&vp);

  //set the scrollbar offsets
  if(showScrollbarH) fragPepA.posX=0-(int)(scrollOffsetH*scrollJumpH);
  else fragPepA.posX=0;
  if(showScrollbarV) fragPepA.posY=0-(int)(scrollOffsetV*scrollJumpV);
  else fragPepA.posY=0;
  if(cbPepA.checked) fragPepA.render();
  if(cbPepB.active && cbPepB.checked) {
    if(showScrollbarH) fragPepB.posX=0-(int)(scrollOffsetH*scrollJumpH);
    else fragPepB.posX=0;
    if(showScrollbarV) fragPepB.posY=0-(int)(scrollOffsetV*scrollJumpV);
    else fragPepB.posY=0;
    if(cbPepA.checked) fragPepB.posY+=(fragPepA.size()+1)*14+23;
    fragPepB.render();
  }

  //expand viewport for scrollbars
  vp.w=origVP.w-posX-4;
  vp.h=origVP.h-posY-115;
  SDL_RenderSetViewport(display->renderer,&vp);

  //draw the scrollbar
  if(showScrollbarV){
    r.x=vp.w-8;
    r.h=thumbHeightV;
    r.w=6;
    r.y=scrollOffsetV;
    SDL_SetRenderDrawColor(display->renderer,185,185,185,255);
    SDL_RenderFillRect(display->renderer,&r);
  }

  //draw the scrollbar
  if(showScrollbarH){
    r.x=scrollOffsetH;
    r.h=6;
    r.w=thumbHeightH;
    r.y=vp.h-8;
    SDL_SetRenderDrawColor(display->renderer,185,185,185,255);
    SDL_RenderFillRect(display->renderer,&r);
  }

  //reset viewport to full window
  SDL_RenderSetViewport(display->renderer,&origVP);

  font->fontSize=fontSize;
  
}

void CPeptideBox::setDisplay(CDisplay* d){
  display = d;
  for(int i=0;i<3;i++){
    for(int j=0;j<6;j++){
      cbIons[i][j].setDisplay(d);
    }
  }
  cbPepA.setDisplay(d);
  cbPepB.setDisplay(d);
  fragPepA.setDisplay(d);
  fragPepB.setDisplay(d);
}

void CPeptideBox::setFocus(CActiveFocus* f){
  activeFocus = f;
  for(int i=0;i<3;i++){
    for(int j=0;j<6;j++){
      cbIons[i][j].setFocus(f);
    }
  }
  cbPepA.setFocus(f);
  cbPepB.setFocus(f);
}

void CPeptideBox::setFont(CFont* f){
  font = f;
  for(int i=0;i<3;i++){
    for(int j=0;j<6;j++){
      cbIons[i][j].setFont(f);
    }
  }
  cbPepA.setFont(f);
  cbPepB.setFont(f);
  fragPepA.setFont(f);
  fragPepB.setFont(f);
}

void CPeptideBox::setPSM(kvPSM& psm){
  size_t i,j;
  char str[6];

  rLinkA.x=-1;
  rLinkB.x=-1;

  cbPepA.caption.clear();
  for(i=0;i<psm.peptideA.size();i++){
    cbPepA.caption+=psm.peptideA[i];
    if(psm.linkA==(int)i){
      rLinkA.x=cbPepA.posX+cbPepA.sz+cbPepA.getCaptionWidth();
      rLinkA.y=cbPepA.posY+11;
      rLinkA.h=4;
    }
    if(psm.linkType==1 && psm.linkB==(int)i){
      rLinkB.x=cbPepA.posX+cbPepA.sz+cbPepA.getCaptionWidth();
      rLinkB.y=cbPepA.posY+15;
      rLinkB.h=-4;
    }
    for(j=0;j<psm.modA->size();j++){
      if(psm.modA->at(j).pos==i){
        sprintf(str,"[%.0lf]",psm.modA->at(j).mass);
        cbPepA.caption+=str;
      }
    }
  }

  cbPepA.active=true;
  cbPepA.checked=true;
  if(psm.linkType==1) fragPepA.setPeptide(psm.peptideA,1,psm.linkA,psm.linkB,psm.compMassA);
  else fragPepA.setPeptide(psm.peptideA,1,psm.linkA,-1,psm.compMassA);
  for(i=0;i<psm.modA->size();i++){
    fragPepA.addMod(psm.modA->at(i).pos,psm.modA->at(i).mass);
  }

  cbPepB.checked=true;
  if(psm.linkType==2){
    cbPepB.caption.clear();
    for(i=0;i<psm.peptideB.size();i++){
      cbPepB.caption+=psm.peptideB[i];
      if(psm.linkB==(int)i){
        rLinkB.x=cbPepB.posX+cbPepB.sz+cbPepB.getCaptionWidth();
        rLinkB.y=cbPepB.posY-5;
        rLinkB.h=4;
      }
      for(j=0;j<psm.modB->size();j++){
        if(psm.modB->at(j).pos==i){
          sprintf(str,"[%.0lf]",psm.modB->at(j).mass);
          cbPepB.caption+=str;
        }
      }
    }
    cbPepB.active=true;
    cbPepB.checked=true;
    fragPepB.setPeptide(psm.peptideB,1,psm.linkB,-1,psm.compMassB);
    for(i=0;i<psm.modB->size();i++){
      fragPepB.addMod(psm.modB->at(i).pos,psm.modB->at(i).mass);
    }
  } else {
    cbPepB.active=false;
  }

  fixLayout();

}

bool CPeptideBox::showPeptide(bool alpha){
  if(alpha){
    if(cbPepA.checked) return true;
  } else {
    if(cbPepB.active && cbPepB.checked) return true;
  }
  return false;
}

