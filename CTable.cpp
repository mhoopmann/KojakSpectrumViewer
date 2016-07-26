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

#include "CTable.h"

char  CTable::sortDataType;
int   CTable::sortIndex;
bool  CTable::sortDir;

CTable::CTable(){
  activeFocus=NULL;
  display=NULL;
  font=NULL;

  posX=0;
  posY=0;
  szX=0;
  szY=0;

  showScrollbarV=false;
  scrollLockV=false;
  scrollOffsetV=0;
  bumpOffsetV=0;

  showScrollbarH=false;
  scrollLockH=false;
  scrollOffsetH=0;
}

CTable::~CTable(){
  activeFocus=NULL;
  display=NULL;
  font=NULL;
}

int CTable::addColumn(string str, char dataType, bool visible){
  kvTableColumn c;
  c.dataType=dataType;
  c.header=str;
  c.ID=(int)columns.size();
  c.visible=visible;
  
  int fontSize=font->fontSize;
  font->fontSize=12;
  c.width=font->getStringWidth(str)+4;
  font->fontSize=fontSize;
  
  columns.push_back(c);
  return c.ID;
}

void CTable::addDataPoint(size_t row, kvTableDP& dp){
  char str[32];
  int w;
  int fontSize=font->fontSize;
  font->fontSize=12;
  switch(columns[dp.ID].dataType){
  case 0:
    sprintf(str,"%d",dp.iVal);
    w=font->getStringWidth(str)+4;
    if(w>columns[dp.ID].width) columns[dp.ID].width=w;
    break;
  case 1:
    if(dp.dVal<0.0001) sprintf(str,"%.4g",dp.dVal);
    else sprintf(str, "%.4lf", dp.dVal);
    w=font->getStringWidth(str)+4;
    if(w>columns[dp.ID].width) columns[dp.ID].width=w;
    break;
  default:
    w=font->getStringWidth(dp.sVal)+4;
    if(w>columns[dp.ID].width) columns[dp.ID].width=w;
    break;
  }
  font->fontSize=fontSize;
  rows[row].dp->push_back(dp);
  rowsFilt[row].dp->push_back(dp);
}

void CTable::addRow(size_t index){
  kvTableRow r;
  r.psmID=index;
  rows.push_back(r);
  rowsFilt.push_back(r);
}

void CTable::applyFilter(kvFilter f){
  size_t i,j;
  int colID;
  vector<kvTableRow>  r;

  //Get the column we are filtering on
  for(i=0; i<columns.size(); i++){
    if(columns[i].header.compare(f.colID)==0) break;
  }
  if(i==columns.size()) return; //case of column doesn't exist
  colID=columns[i].ID;

  for(i=0; i<rowsFilt.size(); i++){
    for(j=0; j<rowsFilt[i].dp->size(); j++){
      if(rowsFilt[i][j].ID==colID) break;
    }
    if(j==rowsFilt[i].dp->size()) continue;
    switch(f.filter){
    case 0:
      if(f.type==0){
        if(rowsFilt[i][j].iVal>f.iLow) r.push_back(rowsFilt[i]);
      } else {
        if(rowsFilt[i][j].dVal>f.dLow) r.push_back(rowsFilt[i]);
      }
      break;
    case 1:
      if(f.type==0){
        if(rowsFilt[i][j].iVal<f.iLow) r.push_back(rowsFilt[i]);
      } else {
        if(rowsFilt[i][j].dVal<f.dLow) r.push_back(rowsFilt[i]);
      }
      break;
    case 2:
      if(f.type==0){
        if(rowsFilt[i][j].iVal>=f.iLow && rowsFilt[i][j].iVal<=f.iHigh) r.push_back(rowsFilt[i]);
      } else {
        if(rowsFilt[i][j].dVal>=f.dLow && rowsFilt[i][j].dVal<=f.dHigh) r.push_back(rowsFilt[i]);
      }
      break;
    case 3:
      if(rowsFilt[i][j].sVal.find(f.sLow)!=string::npos) r.push_back(rowsFilt[i]);
      break;
    case 4:
      if(rowsFilt[i][j].sVal.compare(f.sLow)==0) r.push_back(rowsFilt[i]);
      break;
    default:
      break;
    }
  }

  rowsFilt=r;
  fixLayout();

}

void CTable::clear(){
  columns.clear();
  rows.clear();

  showScrollbarV=false;
  scrollLockV=false;
  scrollOffsetV=0;

  showScrollbarH=false;
  scrollLockH=false;
  scrollOffsetH=0;
}

void CTable::clearFilter(){
  rowsFilt=rows;
  fixLayout();
}

kvTableColumn& CTable::col(size_t index){
  return columns[index];
}

void CTable::fixLayout(){
  int contentSize;
  int scrollTrackSpace;
  int scrollThumbSpace;
  int viewSize;
  
  //Vertical Scrolling
  viewSize = szY-26;
  contentSize = (int)rowsFilt.size()*16; //size of entire text content
  if(viewSize>0 && contentSize>viewSize) {
    showScrollbarV=true;
    
    thumbHeightV = viewSize*viewSize/contentSize;
    if(thumbHeightV<8) thumbHeightV=8;
    thumbMaxV = viewSize-thumbHeightV;

    scrollTrackSpace = contentSize-viewSize;
    scrollThumbSpace = viewSize-thumbHeightV;
    scrollJumpV = (double)scrollTrackSpace/scrollThumbSpace;
    scrollOffsetV = 0;
    stepV=(int)((viewSize-16)/scrollJumpV);
    if(stepV<1) stepV=1;
    bumpV=viewSize-16;
    bumpOffsetV=0;
  } else showScrollbarV=false;

  //Horizontal Scrolling
  viewSize = szX-10;
  contentSize = (int)columns.size(); //size of maximum text line
  for(size_t i=0;i<columns.size();i++) contentSize+=columns[i].width;
  if(viewSize>0 && contentSize>viewSize) {
    showScrollbarH=true;
    
    thumbHeightH = viewSize*viewSize/contentSize;
    thumbMaxH = viewSize-thumbHeightH;

    scrollTrackSpace = contentSize-viewSize;
    scrollThumbSpace = viewSize-thumbHeightH;
    scrollJumpH = (double)scrollTrackSpace/scrollThumbSpace;
    scrollOffsetH = 0;
    stepH=(int)((viewSize-10)/scrollJumpH);
    if(stepH<1) stepH=1;
  } else showScrollbarH=false;
 
}

int CTable::getColumn(char* str){
  string s=str;
  return getColumn(s);
}

int CTable::getColumn(string str){
  size_t i;
  for(i=0;i<columns.size();i++){
    if(columns[i].header.compare(str)==0) break;
  }
  if(i==columns.size()) return -1;
  return (int)i;
}

int CTable::getPSMID(size_t index){
  if(index>=rowsFilt.size()) return -1;
  return (int)rowsFilt[index].psmID;
}

int CTable::logic(int mouseX, int mouseY, int mouseButton, bool mouseButton1){

  //0 = no logic occurred
  //1 = scrollbar activated
  //2 = item selected

  if(mouseButton1 && showScrollbarV){
    //grabbed scrollbar
    if(mouseX>=szX-8 && mouseX<=szX-2 && mouseY>=posY+16+scrollOffsetV && mouseY<=posY+16+scrollOffsetV+thumbHeightV){
      if(scrollLockV){
        scrollOffsetV+=(mouseY-lastMouseY);
        bumpOffsetV=0;
        if(scrollOffsetV>thumbMaxV) scrollOffsetV=thumbMaxV;
        if(scrollOffsetV<0) scrollOffsetV=0;
        lastMouseY=mouseY;
      } else {
        scrollLockV=true;
        lastMouseY=mouseY;
      }
      return 1;
    } else if(scrollLockV){
      scrollOffsetV+=(mouseY-lastMouseY);
      bumpOffsetV=0;
      if(scrollOffsetV>thumbMaxV) scrollOffsetV=thumbMaxV;
      if(scrollOffsetV<0) scrollOffsetV=0;
      lastMouseY=mouseY;
      return 1;
    }
  } 

  //clicked scroll lane (causes jump)
  if(mouseButton && showScrollbarV){
    if(mouseX>=szX-8 && mouseX<=szX-2 && (mouseY>=posY+16 && mouseY<=posY+szY-10)){
      if(mouseY<posY+16+scrollOffsetV){
        bumpOffsetV-=bumpV;
        while(bumpOffsetV<0){
          bumpOffsetV += (int)(stepV*scrollJumpV);
          scrollOffsetV-=stepV;
        }
        if(scrollOffsetV>thumbMaxV) scrollOffsetV=thumbMaxV;
        if(scrollOffsetV<0) scrollOffsetV=0;
        return 1;
      } else if(mouseY>posY+16+scrollOffsetV+thumbHeightV){
        bumpOffsetV+=bumpV;
        while (bumpOffsetV>stepV*scrollJumpV){
          bumpOffsetV -= (int)(stepV*scrollJumpV);
          scrollOffsetV+=stepV;
        }
        if(scrollOffsetV>thumbMaxV) scrollOffsetV=thumbMaxV;
        if(scrollOffsetV<0) scrollOffsetV=0;
        return 1;
      }
    }
  }

  if(mouseButton==1 && scrollLockV){
    scrollLockV=false;
    return 1;
  }

  if(mouseButton1 && showScrollbarH){
    //grabbed scrollbar
    if(mouseX>=scrollOffsetH && mouseX<=scrollOffsetH+thumbHeightH && mouseY>=posY+szY-8 && mouseY<=posY+szY-2){
      if(scrollLockH){
        scrollOffsetH+=(mouseX-lastMouseX);
        if(scrollOffsetH>thumbMaxH) scrollOffsetH=thumbMaxH;
        if(scrollOffsetH<0) scrollOffsetH=0;
        lastMouseX=mouseX;
      } else {
        scrollLockH=true;
        lastMouseX=mouseX;
      }
      return 1;
    } else if(scrollLockH){
      scrollOffsetH+=(mouseX-lastMouseX);
      if(scrollOffsetH>thumbMaxH) scrollOffsetH=thumbMaxH;
      if(scrollOffsetH<0) scrollOffsetH=0;
      lastMouseX=mouseX;
      return 1;
    }
  } 

  //clicked scroll lane (causes jump)
  if(mouseButton && showScrollbarH){
    if(mouseX>=posX && mouseX<=posX+szX-10 && (mouseY>=posY+szY-8 && mouseY<=posY+szY-2)){
      if(mouseX<posX+scrollOffsetH){
        scrollOffsetH-=stepH;
        if(scrollOffsetH>thumbMaxH) scrollOffsetH=thumbMaxH;
        if(scrollOffsetH<0) scrollOffsetH=0;
        return 1;
      } else if(mouseX>posX+scrollOffsetH+thumbHeightH){
        scrollOffsetH+=stepH;
        if(scrollOffsetH>thumbMaxH) scrollOffsetH=thumbMaxH;
        if(scrollOffsetH<0) scrollOffsetH=0;
        return 1;
      }
    }
  }

  if(mouseButton==1 && scrollLockH){
    scrollLockH=false;
    return 1;
  }

  if(mouseButton==1 && mouseX>=posX && mouseX<=(posX+szX-10) && mouseY>=posY+16 && mouseY<=(posY+szY-10)){
    int i=mouseY-posY-16+(int)(scrollOffsetV*scrollJumpV+bumpOffsetV);
    i/=16;
    if(i<(int)rowsFilt.size()) selected = i;
    return 2;
  } 

  scrollLockH=false;
  scrollLockV=false;

  return 0; 
}

bool CTable::render(){
  SDL_Rect r,r2;
  SDL_Rect origVP;
  SDL_Rect vp;
  size_t i,j,k;
  char str[32];
  bool bSelected=false;
  int fontSize=font->fontSize;

  SDL_RenderGetViewport(display->renderer,&origVP);

  //Reset the viewport to just this text box;
  vp.x=posX;
  vp.y=posY;
  vp.w=szX;
  vp.h=szY;
  SDL_RenderSetViewport(display->renderer,&vp);

  //draw background
  r=vp;
  r.x=0;
  r.y=0;
  SDL_SetRenderDrawColor(display->renderer,colors[1].r,colors[1].g,colors[1].b,255);
  SDL_RenderFillRect(display->renderer,&r);

  //Display active column headers
  font->fontSize=12;
  r.x=0-(int)(scrollOffsetH*scrollJumpH);
  r.y=0;
  r.h=15;
  SDL_SetRenderDrawColor(display->renderer, colors[2].r, colors[2].g, colors[2].b, 255);
  for(i=0;i<columns.size();i++){

    if(r.x>vp.w) break;
    if(!columns[i].visible) continue;
    if(r.x+columns[i].width+1<0){
      r.x+=columns[i].width+1;
      continue;
    }

    //draw column background
    r.w=columns[i].width;
    SDL_RenderFillRect(display->renderer,&r);

    //draw column header
    font->render(r.x+2,r.y+2,columns[i].header,txtColors[0]);
    r.x+=columns[i].width+1;
  }
  r.x--;
  r.w=10;
  SDL_RenderFillRect(display->renderer,&r);

  //New viewport for scrollable content
  //also excludes area that scroll bars will occupy
  vp.w -= 10;
  vp.y += 16;
  vp.h -= 26;
  SDL_RenderSetViewport(display->renderer,&vp);

  //Display table contents
  SDL_SetRenderDrawColor(display->renderer, colors[0].r, colors[0].g, colors[0].b, 255);
  for(j=0;j<rowsFilt.size();j++){

    r.x=0-(int)(scrollOffsetH*scrollJumpH);
    r.y=(int)j*16-(int)(scrollOffsetV*scrollJumpV+bumpOffsetV);
    if(r.y>vp.h) break;
    if(r.y+16<0) continue;
    
    if(selected == (int)j) bSelected=true;
    else bSelected=false;

    if(bSelected){
      r2=vp;
      r2.y=r.y;
      r2.h=15;
      SDL_SetRenderDrawColor(display->renderer, colors[3].r, colors[3].g, colors[3].b, 205);
      SDL_RenderFillRect(display->renderer,&r2);
      SDL_SetRenderDrawColor(display->renderer, colors[0].r, colors[0].g, colors[0].b, 255);
    }     

    for(i=0;i<columns.size();i++){
  
      if(r.x>vp.w) break;
      if(!columns[i].visible) continue;
      if(r.x+columns[i].width+1<0){
        r.x+=columns[i].width+1;
        continue;
      }

      //draw column background
      r.w=columns[i].width;
      if(!bSelected) SDL_RenderFillRect(display->renderer,&r);

      //find datapoint
      for(k=0;k<rowsFilt[j].dp->size();k++){
        if(rowsFilt[j][k].ID==columns[i].ID) break;
      }
      if(k==rowsFilt[j].dp->size()) {
        r.x+=columns[i].width+1;
        continue;
      }

      //draw value
      switch(columns[i].dataType){
      case 0:
        sprintf(str,"%d",rowsFilt[j][k].iVal);
        font->render(r.x+2,r.y+2,str,txtColors[1]);
        break;
      case 1:
        if(rowsFilt[j][k].dVal<0.0001) sprintf(str, "%.4g", rowsFilt[j][k].dVal);
        else sprintf(str, "%.4lf", rowsFilt[j][k].dVal);
        font->render(r.x+2,r.y+2,str,txtColors[1]);
        break;
      default:
        font->render(r.x+2,r.y+2,rowsFilt[j][k].sVal,txtColors[1]);
        break;
      }

      r.x+=columns[i].width+1;

    }
  }

  //expand viewport for scrollbars
  vp.w+=10;
  vp.h+=10;
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
  return true;
}

void CTable::setDisplay(CDisplay* d){
  display = d;
  colors[0]=d->pal.table[0];
  colors[1]=d->pal.table[1];
  colors[2]=d->pal.table[2];
  colors[3]=d->pal.table[3];
  txtColors[0]=d->pal.txtTable[0];
  txtColors[1]=d->pal.txtTable[1];
}

void CTable::setFocus(CActiveFocus* f){
  activeFocus = f;
}

void CTable::setFont(CFont* f){
  font = f;
}

size_t CTable::size(bool col){
  if(col) return columns.size();
  else return rowsFilt.size();
}

void CTable::sort(string colID, bool highToLow){

  size_t i;
  for(i=0;i<columns.size();i++){
    if(columns[i].header.compare(colID)==0) break;
  }
  if(i==columns.size()) i=0;
  sortIndex=columns[i].ID;
  sortDir=highToLow;
  sortDataType=columns[i].dataType;

  qsort(&rowsFilt[0],rowsFilt.size(),sizeof(kvTableRow),compar);
  selected=0;
}

int CTable::compar(const void* p1, const void* p2){

  const kvTableRow r1 = *(kvTableRow*)p1;
  const kvTableRow r2 = *(kvTableRow*)p2;

  size_t i;
  for(i=0;i<r1.dp->size();i++){
    if(r1.dp->at(i).ID==sortIndex) break;
  }
  if(i==r1.dp->size()) return 1;

  size_t j;
  for(j=0;j<r2.dp->size();j++){
    if(r2.dp->at(j).ID==sortIndex) break;
  }
  if(j==r2.dp->size()) return -1;

  switch(sortDataType){
  case 0:
    if(sortDir){
      if(r1.dp->at(i).iVal < r2.dp->at(j).iVal) return 1;
      if(r1.dp->at(i).iVal > r2.dp->at(j).iVal) return -1;
    } else {
      if(r1.dp->at(i).iVal < r2.dp->at(j).iVal) return -1;
      if(r1.dp->at(i).iVal > r2.dp->at(j).iVal) return 1;
    }
    return 0;
  case 1:
    if(sortDir){
      if(r1.dp->at(i).dVal < r2.dp->at(j).dVal) return 1;
      if(r1.dp->at(i).dVal > r2.dp->at(j).dVal) return -1;
    } else {
      if(r1.dp->at(i).dVal < r2.dp->at(j).dVal) return -1;
      if(r1.dp->at(i).dVal > r2.dp->at(j).dVal) return 1;
    }
    return 0;
  default:
    if(sortDir) return -r1.dp->at(i).sVal.compare(r2.dp->at(j).sVal);
    return r1.dp->at(i).sVal.compare(r2.dp->at(j).sVal);
  }

}
