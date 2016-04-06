#include "CDisplay.h"

CDisplay::CDisplay(){
  screenHeight=600;
  screenWidth=800;
  renderer=NULL;
  //screenSurface = NULL;
  window = NULL;

  readPalette();

}

CDisplay::~CDisplay(){
  if(renderer!=NULL) SDL_DestroyRenderer(renderer);
  renderer=NULL;
  //if(screenSurface != NULL) SDL_FreeSurface(screenSurface);
	//screenSurface = NULL;
  if(window != NULL) SDL_DestroyWindow(window);
  window = NULL;
  SDL_Quit();
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
	}	else	{
    //Set texture filtering to linear
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) ) {
			printf( "Warning: Linear texture filtering not enabled!" );
		}

		//Create window
		window = SDL_CreateWindow( "Kojak Spectrum Viewer", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screenWidth, screenHeight, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE );
		if( window == NULL )	{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}	else	{
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

	return success;
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
    else if(object.compare("toolbar.color")==0) pal.toolbar[0]=extractColors(rgb);
    else if(object.compare("toolbar.separator")==0) pal.toolbar[1]=extractColors(rgb);
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