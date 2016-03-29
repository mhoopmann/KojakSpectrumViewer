#ifndef _CFILEOPEN_H
#define _CFILEOPEN_H

#include "CActiveFocus.h"
#include "CButton.h"
#include "CDisplay.h"
#include "CList.h"
#include "CTextBox.h"
#include "dirent.h"

#ifdef _MSC_VER
#include <direct.h>
#define getcwd(a,b) _getcwd(a,b)
#endif

using namespace std;

typedef struct kvFileElement {
  char type;    //0=folder, 1=file
  string ext;   //extension (no decimal)
  string name;  //includes extension
} kvFileElement;

class CFileOpen{
public:

  CFileOpen();
  ~CFileOpen();
  
  char cwd[1024];
  char fileName[256];

  void fixLayout   ();
  void init        ();
  int  logic       (int mouseX, int mouseY, int mouseButton, bool mouseButton1);
  void render      ();

  void setDisplay  (CDisplay* d);
  void setFocus    (CActiveFocus* f);
  void setFont     (CFont* f);

private:

  CActiveFocus* activeFocus;
  CDisplay*     display;
  CFont*        font;

  CList listDir;
  CList listFile;
  CTextBox tbPath;

  int mode;

  CButton butOpen;
  CButton butRecent;
  CButton butBack;
  CButton butDirUp;
  CButton butLoad;

  vector<kvFileElement> vFileList;

  void buildFileList(char* path);
  

};

#endif