#ifndef _CDATASET_H
#define _CDATASET_H

#include <vector>

using namespace std;

enum eDSType{
  grBar,
  grLine,
  grPoint
};

typedef struct sDataPoint{
  double x;
  double y;
} sDataPoint;

class CDataSet {
public:

  CDataSet();
  CDataSet(const CDataSet& d);
  ~CDataSet();
  
  CDataSet& operator=(const CDataSet& d);
  sDataPoint& operator[](size_t index);

  eDSType type;

  void add(sDataPoint p);
  void add(double x, double y);
  void clear();
  int findMass(double mass);
  double getMaxX();
  double getMaxY();
  double getMinX();
  double getMinY();
  void getTolerance(double& val, char& unit);
  void setTolerance(double value, char unit);
  size_t size();

private:
  vector<sDataPoint>* data;

  char tolUnit; //0=Da, 1=ppm
  double tol;
  double minX,maxX;
  double minY,maxY;

};

#endif