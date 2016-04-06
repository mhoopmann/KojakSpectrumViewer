#ifndef _CTABLE_H
#define _CTABLE_H

#include "CActiveFocus.h"
#include "CDisplay.h"
#include "CFont.h"
#include <string>
#include <vector>

using namespace std;

typedef struct kvTableColumn {
  char dataType;  //0=int, 1=double, 2=string
  bool visible;
  int ID;
  int width;
  string header;
} kvTableColumn;

typedef struct kvTableDP {
  int ID;   //matches to kvTableColumn.ID
  int iVal;
  double dVal;
  string sVal;
  void clear(){
    ID=0;
    iVal=0;
    dVal=0;
    sVal.clear();
  }
} kvTableDP;

typedef struct kvTableRow {
  size_t psmID;
  vector<kvTableDP>* dp;
  kvTableRow(){
    psmID = 0;
    dp = new vector<kvTableDP>;
  }
  kvTableRow(const kvTableRow& d){
    psmID = d.psmID;
    dp = new vector<kvTableDP>;
    for(size_t i=0;i<d.dp->size();i++) dp->push_back(d.dp->at(i));
  }
  ~kvTableRow(){
    delete dp;
  }
  kvTableRow& operator=(const kvTableRow& d){
    if(this!=&d){
      psmID = d.psmID;
      delete dp;
      dp = new vector<kvTableDP>;
      for(size_t i=0;i<d.dp->size();i++) dp->push_back(d.dp->at(i));
    }
    return *this;
  }
  kvTableDP& operator[](const size_t& index){
    return dp->at(index);
  }
} kvTableRow;


class CTable {
public:

  int posX;
  int posY;
  int szX;
  int szY;
  
  int selected;

  CTable();
  ~CTable();

  int   addColumn   (string str, char dataType, bool visible=true);
  void  addDataPoint(size_t row, kvTableDP& dp);
  void  addRow      (size_t index);
  void  clear       ();

  kvTableColumn& col(size_t index);

  void  fixLayout();

  int getColumn(char* str);
  int getColumn(string str);

  int logic(int mouseX, int mouseY, int mouseButton, bool mouseButton1);
  bool render();
  void setDisplay(CDisplay* d);
  void setFocus(CActiveFocus* f);
  void setFont(CFont* f);
  size_t size(bool col=false);
  void sort(string colID, bool highToLow=true);

private:

  CActiveFocus* activeFocus;
  CDisplay*     display;
  CFont*        font;

  vector<kvTableColumn> columns;
  vector<kvTableRow>  rows; 

  int     lastMouseY;
  double  scrollJumpV;
  bool    scrollLockV;
  int     scrollOffsetV;
  bool    showScrollbarV;
  int     thumbHeightV;
  int     thumbMaxV;

  int     lastMouseX;
  double  scrollJumpH;
  bool    scrollLockH;
  int     scrollOffsetH;
  bool    showScrollbarH;
  int     thumbHeightH;
  int     thumbMaxH;

  static char sortDataType;
  static int  sortIndex;
  static bool sortDir; //true = high to low

  static int compar(const void* p1, const void* p2);

};

#endif
