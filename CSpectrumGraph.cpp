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

#include "CSpectrumGraph.h"

CSpectrumGraph::CSpectrumGraph(){
  font=NULL;
  display=NULL;
  input=NULL;
  szX=128;
  szY=128;
  posX=0;
  posY=0;
  fontSize=10;
  lineWidth=1;
  lowX=100;
  highX=1000;
  lowY=0;
  highY=1000;
  zoomLock=false;
  gridSize=6;
  
  int i;
  gridX=szX/gridSize+1;
  gridY=szY/gridSize+1;
  textGrid = new bool*[gridX];
  for (i = 0; i < gridX; i++){
    textGrid[i] = new bool[gridY];
  }
}

CSpectrumGraph::CSpectrumGraph(CDisplay* d, CInput* inp){
  font=NULL;
  display=d;
  input=inp;
  szX=128;
  szY=128;
  posX=0;
  posY=0;
  fontSize=10;
  lineWidth=1;
  lowX=100;
  highX=1000;
  lowY=0;
  highY=1000;
  zoomLock=false;
  gridSize=8;

  int i;
  gridX = szX / gridSize+1;
  gridY = szY / gridSize+1;
  textGrid = new bool*[gridX];
  for (i = 0; i < gridX; i++){
    textGrid[i] = new bool[gridY];
  }

}

CSpectrumGraph::~CSpectrumGraph(){
  display=NULL;
  input=NULL;
  for (int i = 0; i < gridX; i++){
    delete [] textGrid[i];
  }
  delete[] textGrid;
}

void CSpectrumGraph::exportPNG(){
  int w,h;
  display->getWindowSize(w,h);
  SDL_Surface* s = SDL_CreateRGBSurface(0, w, h, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
  SDL_RenderReadPixels(display->renderer, NULL, SDL_PIXELFORMAT_ARGB8888, s->pixels, s->pitch);
  SDL_Surface* s2 = SDL_PNGFormatAlpha(s);

  time_t rawtime;
  struct tm * timeinfo;
  char fBuf[96];
  time(&rawtime);
  timeinfo = localtime(&rawtime);
  strftime(fBuf, 96, "spec-%Y%m%d-%H%M%S.png", timeinfo);

  SDL_SavePNG(s2,fBuf);
  SDL_FreeSurface(s2);
  SDL_FreeSurface(s);
}

bool CSpectrumGraph::findSpace(int startX, int startY, int width, int height, int& x, int& y){
  //convert width and height to number of grid spaces
  int w = width / gridSize+1;
  int h = height / gridSize+1;

  //convert start positions to grid coordinates
  int a = (startX - posX) / gridSize;
  int b = (startY - posY) / gridSize;
  int refA=a;
  int refB=b;

  int i,j;
  int vert, hor;
  bool bFill;

  for (hor = 0; hor < 6; hor++){
    for (vert = 0; vert < 6+hor; vert++){

      bFill = false;
      for (j = b-vert; j < b-vert + h; j++){
        if (j<0) {
          bFill=true;
          break;
        }
        for (i = a+hor; i < a+hor + w; i++){
          if (i<0 || i>gridX-1) {
            bFill=true;
            break;
          }
          if (textGrid[i][j]){
            bFill = true;
            break;
          }
        }
        if (bFill) break;
      }
      if (!bFill) {
        a+=hor;
        b-=vert;
        break;
      }

      bFill = false;
      for (j = b - vert; j < b - vert + h; j++){
        if (j<0) {
          bFill=true;
          break;
        }
        for (i = a - hor; i < a - hor + w; i++){
          if (i<0 || i>gridX - 1) {
            bFill=true;
            break;
          }
          if (textGrid[i][j]){
            bFill = true;
            break;
          }
        }
        if (bFill) break;
      }
      if (!bFill) {
        a-=hor;
        b-=vert;
        break;
      }

    }
    if (!bFill) break;
  }

  if (bFill) return false;

  //Mark the new positions as filled
  x = posX + a*gridSize;
  y = posY + b*gridSize;
  for (j = b; j < b + h; j++){
    for (i = a; i < a + w; i++){
      textGrid[i][j]=true;
    }
  }
  return true;
}

int CSpectrumGraph::getSizeX(){
  return szX;
}

int CSpectrumGraph::getSizeY(){
  return szY;
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

void CSpectrumGraph::markGrid(int x1, int y1, int x2, int y2){

  //convert x,y to grid boundaries
  int a1 = (x1-posX) / gridSize;
  int a2 = (x2-posX) / gridSize;
  int b1 = (y1-posY) / gridSize;
  int b2 = (y2-posY) / gridSize;

  int x, y;

  if (a2 < a1) {
    x=a2;
    a2=a1;
    a1=x;
  }
  if (b2 < b1){
    x=b2;
    b2=b1;
    b1=x;
  }

  if (a2>gridX) a2=gridX;
  if (b2>gridY) b2=gridY;

  for (x=a1;x<=a2;x++){
    for (y=b1;y<=b2;y++){
      textGrid[x][y]=true;
    }
  }
}

bool CSpectrumGraph::render(CPeptideBox& p){
  SDL_Rect r;
  size_t i,j;
  int w;
  int k;
  int x,y;
  int yy;
  int x2,y2,m;
  double ppuX,ppuY;  //pixels per unit
  char str[32];
  string st;
  CFragmentLists* f;
  int fs=font->fontSize;

  //Render background
  r.x=posX;
  r.y=posY;
  r.w=szX;
  r.h=szY;
  SDL_SetRenderDrawColor(display->renderer,color[0].r,color[0].g,color[0].b,255);
  SDL_RenderFillRect(display->renderer,&r);

  //Calculate scale to fit window
  ppuX = (double)(szX-20)/(highX-lowX);
  if(highY>0) ppuY = (double)(szY-65)/highY;
  else ppuY=0;

  //reset the grid
  resetGrid();

  //Draw spectrum
  SDL_SetRenderDrawColor(display->renderer,color[1].r,color[1].g,color[1].b,255);
  y=posY+szY-15;
  switch(spectrum.type){
    case grBar:
      for(i=0;i<spectrum.size();i++) {
        if(spectrum[i].x<lowX || spectrum[i].x>highX) continue;
        x=posX+(int)((spectrum[i].x-lowX)*ppuX)+10;
        markGrid(x, y, x, y - (int)(spectrum[i].y*ppuY));
        SDL_RenderDrawLine(display->renderer,x,y,x,y-(int)(spectrum[i].y*ppuY));
      }
      break;
    case grLine:
      x2=0;
      y2=posY+szY-15;
      m=posY+szY-15;
      for(i=0;i<spectrum.size();i++) {
        x=posX+(int)((spectrum[i].x-lowX)*ppuX)+10;
        y=m-(int)(spectrum[i].y*ppuY);
        if(spectrum[i].x<lowX || spectrum[i].x>highX) {
          x2=x;
          y2=y;
          continue;
        }
        markGrid(x2,y2,x,y);
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
  font->fontSize=fontSize;
  y=posY+szY-15;
  if(p.showPeptide(true)){
    f=p.getIonSeries();
    for(i=0;i<6;i++){
      for(j=0;j<3;j++){
        if(!p.checkSeries((int)j,(int)i)) continue;
        for(k=0;k<f->size();k++){
          if(i==0) m=spectrum.findMass(f->getAIon((int)j+1,k));
          else if(i==1) m=spectrum.findMass(f->getBIon((int)j+1,k));
          else if(i==2) m=spectrum.findMass(f->getCIon((int)j+1,k));
          else if(i==3) m=spectrum.findMass(f->getXIon((int)j+1,k));
          else if(i==4) m=spectrum.findMass(f->getYIon((int)j+1,k));
          else if(i==5) m=spectrum.findMass(f->getZIon((int)j+1,k));
          if(m<0) {
            f->setMatch((int)i,(int)j,k,false);
            continue;
          }
          f->setMatch((int)i,(int)j,k,true);
          if(spectrum[m].x<lowX || spectrum[m].x>highX) continue;
          x=posX+(int)((spectrum[m].x-lowX)*ppuX)+10;
          yy = y - (int)(spectrum[m].y*ppuY);
          SDL_SetRenderDrawColor(display->renderer,colorIons[j][i].r,colorIons[j][i].g,colorIons[j][i].b,255);
          for(w=-lineWidth/2;w<lineWidth/2+lineWidth%2;w++){
            SDL_RenderDrawLine(display->renderer,x+w,y,x+w,yy);
          }
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
          if(p.getIonSeries(true)!=NULL) st+=(char)30;
          w= font->getStringWidth(st);
          if (findSpace(x - w/2, yy - 2,w, fontSize, x2, y2)){
            font->render(x2,y2,st,txtColor);

            //draw reference line if necessary
            if (x2 + w<x) {
              x2 += w + 1;
            } else if (x2>x) {
              x2 = x2;
            } else {
              x2 = x;
            }
            if (y2 + fontSize > yy){
              y2 += fontSize / 2;
            } else {
              y2 += fontSize;
            }
            int xDif = x2 - x;
            int yDif = y2 - yy;
            SDL_SetRenderDrawColor(display->renderer, color[1].r,0, color[1].b, 255);
            if (abs(xDif) < abs(yDif) && yDif<-10){
              int g = abs(yDif) - abs(xDif);
              SDL_RenderDrawLine(display->renderer, x, yy - 2, x, yy - g);
              SDL_RenderDrawLine(display->renderer, x, yy - g, x + xDif, yy + yDif);
            } else if (abs(xDif)>w / 2){
              SDL_RenderDrawLine(display->renderer, x, yy - 2, x + xDif, yy + yDif);
            } else if (yDif<-10) {
              SDL_RenderDrawLine(display->renderer, x, yy - 2, x, yy + yDif);
            }
            
          }
        }
      }
    }
  }

  //Color fragment ions, peptide B
  f=p.getIonSeries(true);
  if(f!=NULL && p.showPeptide(false)){
    for(i=0;i<6;i++){
      for(j=0;j<3;j++){
        if(!p.checkSeries((int)j,(int)i)) continue;
        for(k=0;k<f->size();k++){
          if(i==0) m=spectrum.findMass(f->getAIon((int)j+1,k));
          else if(i==1) m=spectrum.findMass(f->getBIon((int)j+1,k));
          else if(i==2) m=spectrum.findMass(f->getCIon((int)j+1,k));
          else if(i==3) m=spectrum.findMass(f->getXIon((int)j+1,k));
          else if(i==4) m=spectrum.findMass(f->getYIon((int)j+1,k));
          else if(i==5) m=spectrum.findMass(f->getZIon((int)j+1,k));
          if(m<0) {
            f->setMatch((int)i,(int)j,k,false);
            continue;
          }
          f->setMatch((int)i,(int)j,k,true);
          if(spectrum[m].x<lowX || spectrum[m].x>highX) continue;
          x=posX+(int)((spectrum[m].x-lowX)*ppuX)+10;
          yy = y - (int)(spectrum[m].y*ppuY);
          SDL_SetRenderDrawColor(display->renderer,colorIons[j][i].r,colorIons[j][i].g,colorIons[j][i].b,255);
          for (w = -lineWidth / 2; w<lineWidth / 2 + lineWidth % 2; w++){
            SDL_RenderDrawLine(display->renderer,x+w,y,x+w,yy);
          }
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
          st+=(char)31;
          w = font->getStringWidth(st);
          if (findSpace(x - w/2, yy - 2, w, fontSize, x2, y2)){
            font->render(x2,y2,st,txtColor);

            //draw reference line if necessary
            if (x2+w<x) {
              x2+=w+1;
            } else if (x2>x) {
              x2 = x2;
            } else {
              x2 = x;
            }
            if (y2 + fontSize > yy){
              y2+=fontSize/2;
            } else {
              y2+=fontSize;
            }
            int xDif = x2 - x;
            int yDif = y2 - yy;
            SDL_SetRenderDrawColor(display->renderer, color[1].r, 0, color[1].b, 255);            
            if (abs(xDif) < abs(yDif) && yDif<-10){
              int g = abs(yDif) - abs(xDif);
              SDL_RenderDrawLine(display->renderer, x, yy - 2, x, yy - g);
              markGrid(x, yy-2, x, yy-g);
              SDL_RenderDrawLine(display->renderer, x, yy - g, x + xDif, yy + yDif);
              markGrid(x, yy - g, x + xDif, yy + yDif);
            } else if (abs(xDif)>w/2){
              SDL_RenderDrawLine(display->renderer, x, yy - 2, x + xDif, yy + yDif);
              markGrid(x, yy - 2, x + xDif, yy + yDif);
            } else if (yDif<-10) {
              SDL_RenderDrawLine(display->renderer, x, yy - 2, x, yy + yDif);
              markGrid(x, yy - 2, x, yy + yDif);
            }
          }
        }
      }
    }
  }

  //Draw x-axis
  if(posX+szX-10>posX+10){
    SDL_SetRenderDrawColor(display->renderer,color[2].r,color[2].g,color[2].b,255);
    SDL_RenderDrawLine(display->renderer,posX+10,posY+szY-15,posX+szX-10,posY+szY-15);
    font->fontSize=10;
    sprintf(str,"%.2lf",lowX);
    font->render(10,posY+szY-12,str,txtColor);
    SDL_RenderDrawLine(display->renderer,posX+10,posY+szY-15,posX+10,posY+szY-13);
    sprintf(str,"%.2lf",highX);
    font->render(posX+szX-font->getStringWidth(str)-10, posY+szY-12, str, txtColor);
    SDL_RenderDrawLine(display->renderer,posX+szX-10,posY+szY-15,posX+szX-10,posY+szY-13);
    for(i=szX/5+posX+10;i<szX+posX-20;i+=szX/5){
      sprintf(str,"%.2lf",(double)i/ppuX+lowX);
      font->render((int)i+10-font->getStringWidth(str)/2,posY+szY-12,str,txtColor);
      SDL_RenderDrawLine(display->renderer,(int)i+10,posY+szY-15,(int)i+10,posY+szY-13);
    }
  }

  //Draw y-axis
  if(posY+szY-15>posY+50){
    SDL_SetRenderDrawColor(display->renderer,color[2].r,color[2].g,color[2].b,255);
    SDL_RenderDrawLine(display->renderer,posX+10,posY+szY-15,posX+10,posY+50);
    SDL_RenderDrawLine(display->renderer,posX+8,posY+50,posX+10,posY+50);
    sprintf(str,"%.1e",highY);
    font->render(2,posY+38,str,txtColor);
  }

  /*
  r.h=fontSize+2;
  r.w=fontSize+2;
  for (y = 0; y<gridY; y++){
    for (x=0;x<gridX;x++){
      if (textGrid[x][y]) continue;
      r.y=posY+y*(fontSize+2);
      r.x=posX+x*(fontSize+2);
      SDL_RenderDrawRect(display->renderer,&r);
    }
  }
  */


  //Draw zoom window
  if(zoomLock){
    r.x=lockPos1;
    r.w=lockPos2-lockPos1;
    r.y=posY+50;
    r.h=szY-65;
    SDL_SetRenderDrawColor(display->renderer,0,0,0,128);
    SDL_RenderFillRect(display->renderer,&r);
    
    SDL_SetRenderDrawColor(display->renderer,0,0,0,255);
    SDL_RenderDrawLine(display->renderer,lockPos1,posY+50,lockPos1,posY+szY-15);
    SDL_RenderDrawLine(display->renderer,lockPos2,posY+50,lockPos2,posY+szY-15);
  }

  font->fontSize=fs;
  f=NULL;
  return true;
}

void CSpectrumGraph::resetGrid(){
  int x, y;
  for (x = 0; x < gridX; x++){
    for (y = 0; y < gridY; y++){
      textGrid[x][y] = false;
    }
  }
}

void CSpectrumGraph::resetView(){
  lowX=spectrum.getMinX()-10;
  if(lowX<0) lowX=0;
  highX=spectrum.getMaxX()+10;
  lowY=0;
  highY=spectrum.getMaxY();
}

void CSpectrumGraph::resize(int sizeX, int sizeY){

  int i;

  szX=sizeX;
  szY=sizeY;

  for (i = 0; i < gridX; i++){
    delete[] textGrid[i];
  }
  delete[] textGrid;
    
  gridX = szX / gridSize + 1;
  gridY = szY / gridSize + 1;
  textGrid = new bool*[gridX];
  for (i = 0; i < gridX; i++){
    textGrid[i] = new bool[gridY];
  }

}

void CSpectrumGraph::setDisplay(CDisplay *d){
  display=d;
  size_t i,j;
  for(i=0;i<3;i++) {
    color[i]=display->pal.spectrum[i];
    for(j=0;j<6;j++) colorIons[i][j]=display->pal.spectrumIons[i][j];
  }
  txtColor=display->pal.txtSpectrum;
}

void CSpectrumGraph::setFont(CFont* f){
  font=f;
}

void CSpectrumGraph::setInput(CInput *inp){
  input=inp;
}



