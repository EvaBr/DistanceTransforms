#include "image.h"
#include <queue>
#include <vector>
#include <math.h>
typedef unsigned char  uint8;
typedef image<uint8> imagetype; 
#include "Distances.hpp"
using namespace std;

class Qelem{
 public:
  double maxvalue;
  double minvalue;
  long index;

  Qelem(long i, double min, double max){maxvalue=max; minvalue=min; index=i;}
};

class ElemComparison{
 public:
   bool operator() (const Qelem& lhs, const Qelem&rhs) const{
     if(lhs.maxvalue==rhs.maxvalue){
       return lhs.minvalue<rhs.minvalue;
     }
     return lhs.maxvalue>rhs.maxvalue;
  } 
};

typedef priority_queue<Qelem, vector<Qelem>, ElemComparison> Pq;




void MBD_exact(imagetype seeds, imagetype cost, double* dt){

  int nelem = seeds.getNelem();
  bool* inQ=new bool[nelem];
  int* dtmin=new int[nelem];
  int* dtmax=new int[nelem];
  Pq Q;

  for(int i=0; i< nelem; i++ ){
    dt[i]=256; //INF
    dtmin[i]=-1;
    dtmax[i]=256;
    inQ[i]=false;
    if(seeds[i]>0){
      dt[i]=0;
      dtmin[i]=cost[i];
      dtmax[i]=cost[i];
      Q.push(Qelem(i,cost[i],cost[i]));
      inQ[i]=true;
    }
  }

  int ni;
  double minVal,maxVal;
  vector<int> neighbors;
  vector<int> toadd;

  while(!Q.empty()){
    Qelem e =Q.top();
    Q.pop();
    
    if(inQ[e.index]){
      inQ[e.index]=false;
      //neighbors = seeds.get_8_neighbors(e.index); 
      seeds.get_26_neighbors_py(e.index, neighbors, toadd);
      // TODO! INDEXING BASED ON DIMENSIONALITY; 
      //       AND NEIGHBORHOOD CHOICE AS A PARAMETER!
      
      for(int i=0; i<(int)neighbors.size(); i++){
	      ni=neighbors[i];
	
      	minVal=min(dtmin[e.index], int(cost[ni]));
      	maxVal=max(dtmax[e.index], int(cost[ni]));
	 
      	if(minVal>dtmin[ni]){
	        dtmin[ni]=minVal;
	        dtmax[ni]=maxVal;

        	if(maxVal-minVal <dt[ni]){
	            dt[ni]=maxVal-minVal;
	        }

        	Q.push(Qelem(ni,minVal,maxVal));
	        inQ[ni]=true;
    	  }
      }
    }
  }
  delete[] inQ;
  delete[] dtmin;
  delete[] dtmax;
}


void GeodesicDT(imagetype seeds, imagetype cost, double* dt){
  
  int nelem = seeds.getNelem();
  bool* inQ=new bool[nelem];
  Pq Q;

  for(int i=0; i< nelem; i++ ){
    dt[i]=100000; //INF
    inQ[i]=false;

    if(seeds[i]>0){
      dt[i]=0;
      Q.push(Qelem(i,0,0));
      inQ[i]=true;
    }
  }

  int ni;
  double fval;
  vector<int> neighbors;
  vector<int> toadd;
  
  while(!Q.empty()){
    Qelem e = Q.top();
    Q.pop();
    
    if(inQ[e.index]){
      inQ[e.index]=false;
      //seeds.get_8_neighbors_py(e.index, neighbors, toadd);
      seeds.get_26_neighbors_py(e.index, neighbors, toadd); /* TODO: use _26 for 3D, but _8 for 2D!*/
      
      for(int i=0; i<(int)neighbors.size(); i++){
	    ni=neighbors[i];
	    fval=dt[e.index] + sqrt(pow(double(cost[ni])-double(cost[e.index]),2) + double(toadd[i])); //abs(double(cost[ni])-double(cost[e.index])) + 1;
	 
    	if(fval<dt[ni]){
	        dt[ni]=fval;
	        Q.push(Qelem(ni,0,fval));
	        inQ[ni]=true;
    	}
      }
    }
  }
  delete[] inQ;
}











void MBD(uint8 * seed, uint8 * cost, int height, int width, int channels, double* dt) {
  /* height = size of 1st dim */
  /* width =  size of 2nd dim */
  /* channels = size of 3rd dim*/


  //Setup input

  /*first arg for imagetype creation needs to be a pointer to uint8 data. How to get that??*/
  imagetype seeds = imagetype(seed, height, width, channels); /*border*/
  imagetype costs = imagetype(cost, height, width, channels); /*input image*/

  //Setup output
  //double* dt in output. Computation done in place

  //Calculate DT
  MBD_exact(seeds, costs, dt);
}



void GEO(uint8 * seed, uint8 * cost, int height, int width, int channels, double* dt) {
  /* height = size of 1st dim */
  /* width =  size of 2nd dim */
  /* channels = size of 3rd dim*/


  //Setup input

  /*first arg for imagetype creation needs to be a pointer to uint8 data. How to get that??*/
  imagetype seeds = imagetype(seed, height, width, channels); /*border*/
  imagetype costs = imagetype(cost, height, width, channels); /*input image*/

  //Calculate DT
  GeodesicDT(seeds, costs, dt);
}
