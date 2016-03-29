#include "CDataSet.h"

CDataSet::CDataSet(){
  data = new vector<sDataPoint>;
  type = grBar;
  minX=0;
  maxX=0;
  minY=0;
  maxY=0;
}

CDataSet::CDataSet(const CDataSet& d){
  type = d.type;
  maxX=d.maxX;
  maxY=d.maxY;
  minX=d.minX;
  minY=d.minY;
  data = new vector<sDataPoint>;
  for(size_t i=0;i<d.data->size();i++) data->push_back(d.data->at(i));
}

CDataSet::~CDataSet(){
  delete data;
}

CDataSet& CDataSet::operator=(const CDataSet& d){
  if(this!=&d){
    type = d.type;
    maxX=d.maxX;
    maxY=d.maxY;
    minX=d.minX;
    minY=d.minY;
    delete data;
    data = new vector<sDataPoint>;
    for(size_t i=0;i<d.data->size();i++) data->push_back(d.data->at(i));
  }
  return *this;
}

sDataPoint& CDataSet::operator [](size_t index){
  return data->at(index);
}

void CDataSet::add(sDataPoint p){
  data->push_back(p);
  if(p.y>maxY) maxY=p.y;
  if(p.y<minY) minY=p.y;
  if(p.x>maxX) maxX=p.x;
  if(p.x<minX) minX=p.x;
}

void CDataSet::add(double x, double y){
  sDataPoint p;
  p.x=x;
  p.y=y;
  data->push_back(p);
  if(y>maxY) maxY=y;
  if(y<minY) minY=y;
  if(x>maxX) maxX=x;
  if(x<minX) minX=x;
}

void CDataSet::clear(){
  data->clear();
  minX=0;
  maxX=0;
  minY=0;
  maxY=0;
}

int CDataSet::findMass(double mass){
  int sz=(int)data->size();
  int lower=0;
  int upper=sz;
  int mid=upper/2;

  if(sz==0) return -1;

  double lb=mass-0.01;
  double ub=mass+0.01;

  //binary search to first mass within boundaries
  while(data->at(mid).x<lb || data->at(mid).x>ub){
		if(lower>=upper) return -1;
    if(mass<data->at(mid).x){
			upper=mid-1;
			mid=(lower+upper)/2;
		} else {
			lower=mid+1;
			mid=(lower+upper)/2;
		}
		if(mid==sz) return -1;
	}

  //refine to tallest peak
  double dMax=data->at(mid).y;
  int iMax=mid;
  int i=mid-1;
  while(i>0){
    if(data->at(i).x<lb) break;
    if(data->at(i).y>dMax){
      dMax=data->at(i).y;
      iMax=i;
    }
    i--;
  }
  i=mid+1;
  while(i<sz){
    if(data->at(i).x>ub) break;
    if(data->at(i).y>dMax){
      dMax=data->at(i).y;
      iMax=i;
    }
    i++;
  }
  return iMax;

}

double CDataSet::getMaxX(){
  return maxX;
}

double CDataSet::getMaxY(){
  return maxY;
}

double CDataSet::getMinX(){
  return minX;
}

double CDataSet::getMinY(){
  return minY;
}

size_t CDataSet::size(){
  return data->size();
}