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
