#include "CActiveFocus.h"
#include "CDisplay.h"
#include "CInput.h"
#include "CViewer.h"

int main( int argc, char* args[] ) {
  
  SDL_Event e;
  CActiveFocus appActiveFocus;

  //Start up SDL and create window
  CDisplay display;
  if(!display.init())	{
    printf( "Failed to initialize!\n" );
    return -1;
  }	


  CInput inp;
  CViewer viewer(&display,&inp);
  appActiveFocus.focus = &viewer;
  viewer.setFocus(&appActiveFocus);
  viewer.init();

  while(viewer.viewerMain()){

    while( SDL_PollEvent( &e ) != 0 )	{  
      switch(e.type){
        case SDL_KEYDOWN:
        case SDL_KEYUP:
        case SDL_MOUSEBUTTONDOWN:
        case SDL_MOUSEBUTTONUP:
          inp.processEvent(e);
          break;
        case SDL_WINDOWEVENT:
          viewer.processEvent(e);
          break;
        default:
          break;
      }
    }
    SDL_RenderPresent(display.renderer);
  }


  return 0;
}
