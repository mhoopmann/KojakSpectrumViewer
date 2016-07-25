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

#ifndef _CFILEOPEN_H
#define _CFILEOPEN_H

#include "CAbout.h"
#include "CActiveFocus.h"
#include "CButton.h"
#include "CDisplay.h"
#include "CGfxCollection.h"
#include "CList.h"
#include "CTextBox.h"

#ifdef _MSC_VER
#include "dirent.h"
#include <direct.h>
#define getcwd(a,b) _getcwd(a,b)
#define chdir(a) _chdir(a)
#else
#include <dirent.h>
#include <unistd.h>
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
  void init        (char* ver, char* bdate);
  int  logic       (int mouseX, int mouseY, int mouseButton, bool mouseButton1);
  void render      ();

  void setDisplay  (CDisplay* d);
  void setFocus    (CActiveFocus* f);
  void setFont     (CFont* f);
  void setGfx      (CGfxCollection* g);

private:

  CActiveFocus* activeFocus;
  CDisplay*     display;
  CFont*        font;

  CAbout aboutDlg;
  CList listDir;
  CList listFile;
  CTextBox tbPath;

  int mode;
  bool bFirst;

  CButton butOpen;
  CButton butRecent;
  CButton butBack;
  CButton butDirUp;
  CButton butLoad;

  vector<kvFileElement> vFileList;

  void buildFileList(char* path);
  

};

#endif
