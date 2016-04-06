#include "CFileOpen.h"

CFileOpen::CFileOpen(){
  activeFocus = NULL;
  display = NULL;
  font = NULL;
  getcwd(cwd,1024);
  //directory = opendir(".");
}

CFileOpen::~CFileOpen(){
  activeFocus = NULL;
  display = NULL;
  font = NULL;
  //closedir(directory);
}

void CFileOpen::buildFileList(char* path){

  vFileList.clear();
  kvFileElement f;
  DIR* directory = opendir(path);
  if(directory==NULL) return;
  dirent* entry = NULL;

  while(entry = readdir(directory)){
    if(entry->d_name[0]=='.') continue;
    f.name=entry->d_name;
    if(entry->d_type==DT_DIR) {
      f.ext.clear();
      f.type=0;
    } else {
      if(f.name.rfind(".")!=string::npos){
        f.ext=f.name.substr(f.name.rfind(".",string::npos));
        if(f.ext.compare(".xml")==0){
          printf("%s: %d\n",&f.name[0],f.name.rfind(".pep.xml"));
          if(f.name.rfind(".pep.xml")==f.name.size()-8){
            f.ext="pep.xml";
          }
        }
      } else {
        f.ext.clear();
      }
      f.type=1;
    }
    if(f.type==0 || f.ext.compare("pep.xml")==0) vFileList.push_back(f);
  }
  closedir(directory);

  listFile.clear();
  size_t i;
  //directories on top
  for(i=0;i<vFileList.size();i++){
    if(vFileList[i].type==0) listFile.addItem(&vFileList[i].name[0],(int)i,0);
  }
  //then files
  for(i=0;i<vFileList.size();i++){
    if(vFileList[i].type==1) listFile.addItem(&vFileList[i].name[0],(int)i);
  }

  fixLayout();

}

void CFileOpen::fixLayout(){
  int h,w;
  display->getWindowSize(w,h);
  tbPath.szX=w-167;
  butDirUp.posX=w-40;
  butLoad.posX=w-74;
  butLoad.posY=h-42;
  listFile.szX=w-listFile.posX-10;
  listDir.szY=h-listDir.posY-52;
  listFile.szY=listDir.szY;

  listFile.fixLayout();
  listDir.fixLayout();
}

void CFileOpen::init(){
  int i;

  butOpen.posX=0;
  butOpen.posY=0;
  butOpen.szX=120;
  butOpen.szY=50;

  butRecent.posX=0;
  butRecent.posY=50;
  butRecent.szX=120;
  butRecent.szY=50;

  butBack.posX=0;
  butBack.posY=100;
  butBack.szX=120;
  butBack.szY=50;

  butOpen.setCaption("Open");
  butRecent.setCaption("Recent");
  butBack.setCaption("Back");
  butLoad.setCaption("Load");

  butLoad.posX=50;
  butLoad.szX=64;
  butLoad.szY=32;
  butLoad.active=false;

  tbPath.posX=130;
  tbPath.posY=20;
  tbPath.szX=596;
  tbPath.szY=20;
  tbPath.noScroll=true;
  tbPath.setSize(14);
  tbPath.addText(cwd);

  butDirUp.posX=731;
  butDirUp.posY=20;
  butDirUp.szX=30;
  butDirUp.szY=26;
  butDirUp.setImage(4);

  listDir.posX=130;
  listDir.posY=60;
  listDir.szX=50;
  listDir.szY=450;
  listDir.setSize(14);
  char drv[12];
  strcpy(drv,"A:");
  ULONG uDriveMask = _getdrives();
  i=1;
  while (uDriveMask) {
    if (uDriveMask & 1) {
      listDir.addItem(drv,i);
      if(_getdrive()==i) listDir.setSelected((int)listDir.size()-1);
    }
    ++drv[0];
    i++;
    uDriveMask >>= 1;
  }

  listFile.posX=190;
  listFile.posY=60;
  listFile.szX=250;
  listFile.szY=450;
  listFile.setSize(14);

  mode=0;
  fixLayout();

}

int CFileOpen::logic(int mouseX, int mouseY, int mouseButton, bool mouseButton1){
  if(butOpen.logic(mouseX,mouseY,mouseButton)) {
    mode=1;
    buildFileList(".");
    return 0;
  }
  if(butBack.logic(mouseX,mouseY,mouseButton)) {
    mode=0;
    return 1;
  }
  if(butDirUp.logic(mouseX,mouseY,mouseButton)){
    _chdir("..");
    buildFileList(".");
    getcwd(cwd,1024);
    tbPath.clear();
    tbPath.addText(cwd);
    return 0;
  }
  if(butLoad.active && butLoad.logic(mouseX,mouseY,mouseButton)){
    strcpy(fileName,&vFileList[listFile.getSelectedItem().value].name[0]);
    return 2;
  }
  switch(listFile.logic(mouseX,mouseY,mouseButton,mouseButton1)){
    case 1:
      return 0;
    case 2:
      if(listFile.getSelected()<0) return 0;
      if(vFileList[listFile.getSelectedItem().value].type==0){
        _chdir(&vFileList[listFile.getSelectedItem().value].name[0]);
        buildFileList(".");
        getcwd(cwd,1024);
        tbPath.clear();
        tbPath.addText(cwd);
        butLoad.active=false;
      }
      butLoad.active=true;
      return 0;
    default:
      break;
  }

  switch(listDir.logic(mouseX,mouseY,mouseButton,mouseButton1)){
    case 1:
      return 0;
    case 2:
      if(listDir.getSelected()<0) return 0;
      if(_chdrive(listDir.getSelectedItem().value)==0) {
        buildFileList(".");
        getcwd(cwd,1024);
        tbPath.clear();
        tbPath.addText(cwd);
      } else {
        listFile.clear();
        tbPath.clear();
      }
      butLoad.active=false;
      return 0;
    default:
      break;
  }
  return 0;
}

void CFileOpen::render(){
  //always full screen
  SDL_Rect r;
  int fontSize = font->fontSize;

  //Clear the entire window
  SDL_RenderGetViewport(display->renderer,&r);
  SDL_SetRenderDrawColor(display->renderer,255,255,255,255);
  SDL_RenderFillRect(display->renderer,&r);

  //Draw the sidebar
  r.w=120;
  SDL_SetRenderDrawColor(display->renderer,128,128,128,255);
  SDL_RenderFillRect(display->renderer,&r);

  //Draw the buttons
  butOpen.render();
  butRecent.render();
  butBack.render();
  
  if(mode==1){
    font->fontSize=14;
    font->render(130,5,"Path:",1);
    font->render(130,45,"Drive:",1);
    font->render(190,45,"Files:",1);
    tbPath.render();
    listDir.render();
    listFile.render();
    butDirUp.render();
    butLoad.render();
    font->fontSize=fontSize;
  }

}

void CFileOpen::setDisplay(CDisplay* d){
  display = d;
  butOpen.setDisplay(d);
  butRecent.setDisplay(d);
  butBack.setDisplay(d);
  butDirUp.setDisplay(d);
  butLoad.setDisplay(d);
  listDir.setDisplay(d);
  listFile.setDisplay(d);
  tbPath.setDisplay(d);
}

void CFileOpen::setFocus(CActiveFocus* f){
  activeFocus = f;
  butOpen.setFocus(f);
  butRecent.setFocus(f);
  butBack.setFocus(f);
  butDirUp.setFocus(f);
  butLoad.setFocus(f);
  tbPath.setFocus(f);
}

void CFileOpen::setFont(CFont* f){
  font = f;
  butOpen.setFont(f);
  butRecent.setFont(f);
  butBack.setFont(f);
  butDirUp.setFont(f);
  butLoad.setFont(f);
  listDir.setFont(f);
  listFile.setFont(f);
  tbPath.setFont(f);
}

void CFileOpen::setGfx(CGfxCollection* g){
  listFile.setGfx(g);
  butDirUp.setGfx(g);
}
