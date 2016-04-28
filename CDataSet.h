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