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

#include "CDisplay.h"

CDisplay::CDisplay(){
  screenHeight=600;
  screenWidth=800;
  renderer=NULL;
  window = NULL;
  cursors[0]=NULL;
  cursors[1]=NULL;
  cursors[2]=NULL;
  cursors[3]=NULL;

  defaultColors();
  readPalette();

}

CDisplay::~CDisplay(){
  if(renderer!=NULL) SDL_DestroyRenderer(renderer);
  renderer=NULL;
  if(window != NULL) SDL_DestroyWindow(window);
  window = NULL;

  if(cursors[0]!=NULL) SDL_FreeCursor(cursors[0]);
  if(cursors[1]!=NULL) SDL_FreeCursor(cursors[1]);
  if(cursors[2]!=NULL) SDL_FreeCursor(cursors[2]);
  if(cursors[3]!=NULL) SDL_FreeCursor(cursors[3]);
  cursors[0]=NULL;
  cursors[1]=NULL;
  cursors[2]=NULL;
  cursors[3]=NULL;

  SDL_Quit();
}

void CDisplay::defaultColors(){
  pal.buttons[0]=extractColors("85,98,112");
  pal.buttons[1]=extractColors("255,156,0");
  pal.buttons[2]=extractColors("217,133,0");
  pal.buttons[3]=extractColors("25,133,125");
  pal.buttons[4]=extractColors("85,98,112");
  pal.checkbox=extractColors("85,98,112");
  pal.ionTable[0]=extractColors("85,98,112");
  pal.ionTable[1]=extractColors("212,215,219");
  pal.ionTable[2]=extractColors("238,238,240");
  pal.list[0]=extractColors("85,98,112");
  pal.list[1]=extractColors("255,156,0");
  pal.list[2]=extractColors("255,156,0");
  pal.toolbar[0]=extractColors("85,98,112");
  pal.toolbar[1]=extractColors("68,74,80");
  pal.slider = extractColors("68,74,80");
  pal.spectrum[0]=extractColors("255,255,255");
  pal.spectrum[1]=extractColors("180,180,180");
  pal.spectrum[2]=extractColors("32,32,32");
  pal.spectrumIons[0][0]=extractColors("125,196,226");
  pal.spectrumIons[1][0]=extractColors("125,196,226");
  pal.spectrumIons[2][0]=extractColors("125,196,226");
  pal.spectrumIons[0][1]=extractColors("85,170,207");
  pal.spectrumIons[1][1]=extractColors("85,170,207");
  pal.spectrumIons[2][1]=extractColors("85,170,207");
  pal.spectrumIons[0][2]=extractColors("53,146,186");
  pal.spectrumIons[1][2]=extractColors("53,146,186");
  pal.spectrumIons[2][2]=extractColors("53,146,186");
  pal.spectrumIons[0][3]=extractColors("255,144,144");
  pal.spectrumIons[1][3]=extractColors("255,144,144");
  pal.spectrumIons[2][3]=extractColors("255,144,144");
  pal.spectrumIons[0][4]=extractColors("255,107,107");
  pal.spectrumIons[1][4]=extractColors("255,107,107");
  pal.spectrumIons[2][4]=extractColors("255,107,107");
  pal.spectrumIons[0][5]=extractColors("166,35,35");
  pal.spectrumIons[1][5]=extractColors("166,35,35");
  pal.spectrumIons[2][5]=extractColors("166,35,35");
  pal.table[0]=extractColors("255,255,255");
  pal.table[1]=extractColors("235,235,235");
  pal.table[2]=extractColors("85,98,112");
  pal.table[3]=extractColors("120,215,208");
  pal.textBox[0]=extractColors("85,98,112");
  pal.textBox[1]=extractColors("25,133,125");
  pal.txtDefault=extractColorsText("44,62,80");
  pal.txtButton[0]=extractColorsText("255,255,255");
  pal.txtButton[1]=extractColorsText("128,128,128");
  pal.txtCheckbox=extractColorsText("44,62,80");
  pal.txtIonTable[0]=extractColorsText("255,255,255");
  pal.txtIonTable[1]=extractColorsText("44,62,80");
  pal.txtIonTable[2]=extractColorsText("44,62,80");
  pal.txtList=extractColorsText("255,255,255");
  pal.txtSpectrum=extractColorsText("44,62,80");
  pal.txtTable[0]=extractColorsText("255,255,255");
  pal.txtTable[1]=extractColorsText("44,62,80");
  pal.txtTextBox=extractColorsText("255,255,255");
}

bool CDisplay::getWindowSize(int& w, int& h){
  SDL_GetWindowSize(window,&screenWidth,&screenHeight);
  w=screenWidth;
  h=screenHeight;
  return true;
}

bool CDisplay::init() {
  //Initialization flag
  bool success = true;

  //Initialize SDL
  if( SDL_Init( SDL_INIT_VIDEO ) < 0 ){
    printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
    success = false;
  } else {
    //Set texture filtering to linear
    if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) ) {
      printf( "Warning: Linear texture filtering not enabled!" );
    }

    //Create window
    window = SDL_CreateWindow( "Kojak Spectrum Viewer", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screenWidth, screenHeight, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE );
    if( window == NULL ) {
      printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
      success = false;
    } else {
      //Get window surface
      //screenSurface = SDL_GetWindowSurface(window);

      renderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
      if( renderer == NULL ) {
        printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
        success = false;
      } else {
        //Initialize renderer color
        SDL_SetRenderDrawColor( renderer, 0xFF, 0xFF, 0xFF, 0xFF );
        SDL_SetRenderDrawBlendMode(renderer,SDL_BLENDMODE_BLEND);
      }

    }
  }

  if(cursors[0]==NULL) cursors[0]=SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW);
  if(cursors[1]==NULL) cursors[1]=SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_WAIT);
  if(cursors[2]==NULL) cursors[2]=SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZEWE);
  if(cursors[3]==NULL) cursors[3]=SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZENS);

  return success;
}

kvColor CDisplay::extractColors(const char* s){
  string str=s;
  return extractColors(str);
}

kvColor CDisplay::extractColors(string& s){
  char* tok;
  kvColor col;

  tok=strtok(&s[0],",");
  col.r=atoi(tok);
  tok=strtok(NULL,",");
  col.g=atoi(tok);
  tok=strtok(NULL,",");
  col.b=atoi(tok);

  return col;
}

int CDisplay::extractColorsText(const char* s){
  string str=s;
  return extractColorsText(str);
}

int CDisplay::extractColorsText(string& s){
  char* tok;
  size_t i;
  kvColor col;

  tok=strtok(&s[0], ",");
  col.r=atoi(tok);
  tok=strtok(NULL, ",");
  col.g=atoi(tok);
  tok=strtok(NULL, ",");
  col.b=atoi(tok);

  for(i=0; i<txtColors.size(); i++){
    if(txtColors[i].r==col.r && txtColors[i].g==col.g && txtColors[i].b==col.b) break;
  }
  if(i==txtColors.size()){
    txtColors.push_back(col);
  }

  return (int)i;
}

void CDisplay::readPalette(){
  FILE* f=fopen("colors.txt","rt");
  if(f==NULL) return;

  char str[256];
  char* tok;
  string object;
  string rgb;
  while(!feof(f)){
    if(fgets(str,256,f)==NULL) continue;
    tok=strtok(str,"= \t\n\r");
    object=tok;
    tok=strtok(NULL,"= \t\n\r");
    rgb=tok;

    if(object.compare("button.color")==0) pal.buttons[0]=extractColors(rgb);
    else if(object.compare("button.highlight")==0) pal.buttons[1]=extractColors(rgb);
    else if(object.compare("button.highlight_border")==0) pal.buttons[2]=extractColors(rgb);
    else if(object.compare("button.pressed")==0) pal.buttons[3]=extractColors(rgb);
    else if(object.compare("button.inactive")==0) pal.buttons[4]=extractColors(rgb);
    else if(object.compare("checkbox.color")==0) pal.checkbox=extractColors(rgb);
    else if(object.compare("ions.header")==0) pal.ionTable[0]=extractColors(rgb);
    else if(object.compare("ions.subheader")==0) pal.ionTable[1]=extractColors(rgb);
    else if(object.compare("ions.cell")==0) pal.ionTable[2]=extractColors(rgb);
    else if(object.compare("list.color")==0) pal.list[0]=extractColors(rgb);
    else if(object.compare("list.highlight")==0) pal.list[1]=extractColors(rgb);
    else if(object.compare("list.selected")==0) pal.list[2]=extractColors(rgb);
    else if(object.compare("toolbar.color")==0) pal.toolbar[0]=extractColors(rgb);
    else if(object.compare("toolbar.separator")==0) pal.toolbar[1]=extractColors(rgb);
    else if(object.compare("slider.color")==0) pal.slider=extractColors(rgb);
    else if(object.compare("spectrum.background")==0) pal.spectrum[0]=extractColors(rgb);
    else if(object.compare("spectrum.graph")==0) pal.spectrum[1]=extractColors(rgb);
    else if(object.compare("spectrum.graph_axis")==0) pal.spectrum[2]=extractColors(rgb);
    else if(object.compare("spectrum.aion_1")==0) pal.spectrumIons[0][0]=extractColors(rgb);
    else if(object.compare("spectrum.aion_2")==0) pal.spectrumIons[1][0]=extractColors(rgb);
    else if(object.compare("spectrum.aion_3")==0) pal.spectrumIons[2][0]=extractColors(rgb);
    else if(object.compare("spectrum.bion_1")==0) pal.spectrumIons[0][1]=extractColors(rgb);
    else if(object.compare("spectrum.bion_2")==0) pal.spectrumIons[1][1]=extractColors(rgb);
    else if(object.compare("spectrum.bion_3")==0) pal.spectrumIons[2][1]=extractColors(rgb);
    else if(object.compare("spectrum.cion_1")==0) pal.spectrumIons[0][2]=extractColors(rgb);
    else if(object.compare("spectrum.cion_2")==0) pal.spectrumIons[1][2]=extractColors(rgb);
    else if(object.compare("spectrum.cion_3")==0) pal.spectrumIons[2][2]=extractColors(rgb);
    else if(object.compare("spectrum.xion_1")==0) pal.spectrumIons[0][3]=extractColors(rgb);
    else if(object.compare("spectrum.xion_2")==0) pal.spectrumIons[1][3]=extractColors(rgb);
    else if(object.compare("spectrum.xion_3")==0) pal.spectrumIons[2][3]=extractColors(rgb);
    else if(object.compare("spectrum.yion_1")==0) pal.spectrumIons[0][4]=extractColors(rgb);
    else if(object.compare("spectrum.yion_2")==0) pal.spectrumIons[1][4]=extractColors(rgb);
    else if(object.compare("spectrum.yion_3")==0) pal.spectrumIons[2][4]=extractColors(rgb);
    else if(object.compare("spectrum.zion_1")==0) pal.spectrumIons[0][5]=extractColors(rgb);
    else if(object.compare("spectrum.zion_2")==0) pal.spectrumIons[1][5]=extractColors(rgb);
    else if(object.compare("spectrum.zion_3")==0) pal.spectrumIons[2][5]=extractColors(rgb);

  }
  fclose(f);
}
