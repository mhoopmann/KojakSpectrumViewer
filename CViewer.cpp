#include "CViewer.h"

CViewer::CViewer(){
  activeFocus=NULL;
  display=NULL;
  input=NULL;
  state=1;
  firstOpen=true;
  quitter=false;
  focused=true;
}

CViewer::CViewer(CDisplay* d, CInput* inp){
  activeFocus=NULL;
  display=d;
  input=inp;
  sg.setDisplay(d);
  sg.setInput(inp);
  state=1;
  firstOpen=true;
  quitter=false;
  focused=true;
}

CViewer::~CViewer(){
  activeFocus=NULL;
  display=NULL;
  input=NULL;
}

void CViewer::fixLayout(){
  int h,w;
  display->getWindowSize(w,h);

  tb.szX=w;
  sliderV.szY=h-tb.szY;
  pepBox.szY=h-tb.szY;
  msgBox.szY=h-msgBox.posY;
  dt.szY=h-dt.posY; 

  pepBox.szX=w-pepBox.posX;

  pepBox.fixLayout();
  msgBox.fixLayout();
  dt.fixLayout();
  fileDlg.fixLayout();

}

void CViewer::init(){
  sg.posY=32;
  sg.szX=500;
  sg.szY=300;  

#ifdef GCC
  gfx.loadGfx("./Fonts/icons.bmp",display->renderer);
#else
  gfx.loadGfx("Fonts\\icons.bmp",display->renderer);
#endif

  font.setRenderer(display->renderer);
#ifdef GCC
  font.loadFont("./Fonts/Carlito-Regular.ttf");
#else
  font.loadFont("Fonts\\Carlito-Regular.ttf");
#endif

  msgBox.setDisplay(display);
  msgBox.setFont(&font);
  msgBox.posX=0;
  msgBox.posY=337;
  msgBox.szX=500;
  msgBox.szY=263;
  msgBox.addText("Testing Message Box");
  msgBox.addText("Testing another line.");
  msgBox.addText("Testing another really, really, really, really, really, really, really, really, really, really, really, really, really, really, really, really, really, really, really, really, really, really, really, really, really, long line.");
  msgBox.addText("Testing another line.");

  dt.setDisplay(display);
  dt.setFont(&font);
  dt.posX=0;
  dt.posY=337;
  dt.szX=500;
  dt.szY=263;

  pepBox.setDisplay(display);
  pepBox.setFont(&font);
  pepBox.posX=505;
  pepBox.posY=32;
  pepBox.szX=295;
  pepBox.szY=568;
  pepBox.init();

  fileDlg.setDisplay(display);
  fileDlg.setFont(&font);
  fileDlg.setGfx(&gfx);
  fileDlg.init();

  aboutDlg.setDisplay(display);
  aboutDlg.setFont(&font);
  aboutDlg.init();
  
  dataIndex=0;
  dt.selected=0;
  sg.setFont(&font);

  sortDlg.setDisplay(display);
  sortDlg.setFont(&font);
  sortDlg.setInput(input);
  sortDlg.init(&dt);

  setDlg.setDisplay(display);
  setDlg.setFont(&font);
  setDlg.setInput(input);
  setDlg.init();

  tb.setDisplay(display);
  tb.setFont(&font);
  tb.setGfx(&gfx);
  tb.setInput(input);
  tb.init();

  sliderH.setDisplay(display);
  sliderH.setFocus(activeFocus);
  sliderH.posX=0;
  sliderH.posY=332;
  sliderH.szY=5;
  sliderH.szX=500;
  sliderH.vertical=false;

  sliderV.setDisplay(display);
  sliderV.setFocus(activeFocus);
  sliderV.posX=500;
  sliderV.posY=32;
  sliderV.szX=5;
  sliderV.szY=568;

  dt.setDisplay(display);
  dt.setFocus(activeFocus);
  dt.posX=0;
  dt.posY=337;
  dt.szX=500;
  dt.szY=263;

}

bool CViewer::logic(){
  //Master controller of all user interaction
  int mouseX;
  int mouseY;
  int mouseButton=0;
  int val;
  bool mouseButton1;
  char str[64];

  if(quitter) return false;
  if(!focused) return true;

  SDL_GetMouseState(&mouseX,&mouseY);
  mouseButton=input->mouseAction();
  mouseButton1=input->getButtonState(0);

  //intercept for other states
  if(state==1){
    val=fileDlg.logic(mouseX,mouseY,mouseButton,mouseButton1);
    switch(val){
    case 1:
      if(!firstOpen) state=0;
      return true;
    case 2:
      state=0;
      dt.clear();
      data.readPepXML(fileDlg.fileName,&dt);
      dataIndex=0;
      dt.selected=0;
      sortDlg.clear();
      sortDlg.init(&dt);
      resetSpectrum();
      firstOpen=false;
      return true;
    default:
      return true;
    }
  }

  if(state==2){
    val=sortDlg.logic(mouseX,mouseY,mouseButton,mouseButton1);
    switch(val){
    case 1:
      state=0;
      return true;
    case 2:
      state=0;
      dataIndex=dt.selected;
      resetSpectrum();
      return true;
    default:
      return true;
    }
  }

  if(state==3){
    val=aboutDlg.logic(mouseX, mouseY, mouseButton, mouseButton1);
    switch(val){
    case 1:
      state=0;
      return true;
    default:
      return true;
    }
  }

  if(state==4){
    val=setDlg.logic(mouseX, mouseY, mouseButton, mouseButton1);
    switch(val){
    case 1:
      state=0;
      return true;
    case 2:
      state=0;
      sg.spectrum.setTolerance(setDlg.tol, setDlg.tolUnit);
      if(setDlg.tolUnit==0) sprintf(str, "Tolerance: %g Da",setDlg.tol);
      else sprintf(str, "Tolerance: %g ppm", setDlg.tol);
      pepBox.metaTol=str;
      return true;
    default:
      return true;
    }
  }

  //only process other components if sliders are not locked
  if(!sliderV.checkLocked() && !sliderH.checkLocked()){

    switch(dt.logic(mouseX,mouseY,mouseButton,mouseButton1)){
      case 1:
        return true;
      case 2:
        dataIndex=dt.selected;
        resetSpectrum();
        return true;
      default:
        break;
    }

    //Process any toolbar actions
    switch(tb.logic(mouseX,mouseY,mouseButton)){
      case 1: //open button
        state=1;
        return true;
      case 2: //prev button
        dataIndex--;
        if(dataIndex<0) dataIndex=0;
        dt.selected=dataIndex;
        resetSpectrum();
        return true;
      case 3: //next button
        dataIndex++;
        if(dataIndex==dt.size(false)) dataIndex=(int)(dt.size(false)-1);
        dt.selected=dataIndex;
        resetSpectrum();
        return true;
      case 4: //about dialog
        state=3;
        return true;
      case 5: //sort/filter
        state=2;
        return true;
      case 6: //anywhere in the toolbar
        return true;
      case 7: //settings dialog
        state=4;
        sg.spectrum.getTolerance(setDlg.tol, setDlg.tolUnit);
        return true;
      default:
        break;
    }

    if(pepBox.logic(mouseX,mouseY,mouseButton,mouseButton1)>0) return true;
    //if(msgBox.logic(mouseX,mouseY,mouseButton,mouseButton1)>0) return true;

    if(sg.logic(mouseX,mouseY,mouseButton,mouseButton1)) {
      return true;
    }

  }

  //always check sliders last?
  val=sliderV.logic(mouseX, mouseY, mouseButton, mouseButton1);
  if(val<10000){
    sg.szX+=val;
    dt.szX+=val;
    dt.fixLayout();
    msgBox.szX+=val;
    msgBox.fixLayout();
    sliderH.szX+=val;
    pepBox.posX=sliderV.posX+sliderV.szX;
    pepBox.szX-=val;
    pepBox.fixLayout();
    return true;
  }
  
  val=sliderH.logic(mouseX, mouseY, mouseButton, mouseButton1);
  if(val<10000){
    sg.szY+=val;
    dt.posY+=val;
    dt.szY-=val;
    dt.fixLayout();
    msgBox.posY+=val;
    msgBox.szY-=val;
    msgBox.fixLayout();    
    return true;
  }

  tb.processInput();
  /*
  int id;
  switch(tb.processInput()){
    case 1:
      id=data.findSpectrum(tb.scanNumber);
      if(id>-1){
        dataIndex=id;
        resetSpectrum();
      }
      return true;
    default:
      break;
  }
  */
  if(input->getKeyState(KEY_Q))return false;
  return true;
}

void CViewer::processEvent(SDL_Event& e){
  switch(e.window.event){
  case SDL_WINDOWEVENT_RESIZED:
    fixLayout();
    break;
  case SDL_WINDOWEVENT_FOCUS_GAINED:
    focused=true;
    break;
  case SDL_WINDOWEVENT_FOCUS_LOST:
    focused=false;
    break;
  case SDL_WINDOWEVENT_CLOSE:
    quitter=true;
    break;
  default:
    break;
  }
}

bool CViewer::render(){
  SDL_Rect r;

  switch(state){
  case 1:
    fileDlg.render();
    return true;
  case 2:
    sortDlg.render();
    return true;
  case 3:
    aboutDlg.render();
    return true;
  case 4:
    setDlg.render();
    return true;
  default:
    break;
  }


  //Clear the entire window
  SDL_RenderGetViewport(display->renderer,&r);
  SDL_SetRenderDrawColor(display->renderer,0,0,0,255);
  SDL_RenderFillRect(display->renderer,&r);

  tb.render();

  sg.render(pepBox);

  //render graph sliders
  sliderH.render();
  sliderV.render();

  //msgBox.render();
  dt.render();

  //Render peaklists
  pepBox.render(0,0);

  return true;
}

void CViewer::resetSpectrum(){
  //fragList.setPeptide(data[dataIndex].peptideA,1);
  int tIndex=dt.getPSMID(dataIndex);
  if(tIndex<0) {
    sg.spectrum.clear();
    sg.resetView();
    pepBox.clear=true;
    return;
  }

  pepBox.clear=false;

  scanNumber = data[tIndex].scanNumber;
  pepBox.setPSM(data[tIndex]);
  Spectrum spec = data.getSpectrum(tIndex);
  pepBox.scanNumber = spec.getScanNumber();
  pepBox.mass = data[tIndex].mass;
  pepBox.charge = (int)data[tIndex].charge;
  sg.spectrum.clear();
  for(int i=0;i<spec.size();i++){
    if(spec[i].intensity==0) continue;
    sg.spectrum.add(spec[i].mz,(double)spec[i].intensity);
  }
  sg.resetView();
}

void CViewer::setDisplay(CDisplay *d){
  display=d;
  sg.setDisplay(d);
  pepBox.setDisplay(d);
  fileDlg.setDisplay(d);
  sortDlg.setDisplay(d);
  aboutDlg.setDisplay(d);
  setDlg.setDisplay(d);
}

void CViewer::setFocus(CActiveFocus* f){
  activeFocus=f;
  tb.setFocus(f);
  pepBox.setFocus(f);
  msgBox.setFocus(f);
  fileDlg.setFocus(f);
  dt.setFocus(f);
  sortDlg.setFocus(f);
  aboutDlg.setFocus(f);
  setDlg.setFocus(f);
}

void CViewer::setInput(CInput* inp){
  input=inp;
  sg.setInput(inp);
  sortDlg.setInput(inp);
  setDlg.setInput(inp);
}

bool CViewer::viewerMain(){
  //Check logic
  if(!logic()) return false;
  //render
  render();
  return true;
}
