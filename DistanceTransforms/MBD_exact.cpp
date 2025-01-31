#include "mex.h"
#include "matrix.h"
//#include <iostream>
//#include <fstream>
#include "image.h"
typedef unsigned char  uint8;
typedef image<uint8> imagetype; 
#include "MBD_exact.h"

using namespace std;

////////////////////////////////////////////////////
// Usage: 
// MBD_exact(seeds, labels, cost)
////////////////////////////////////////////////////


void mexFunction(int nlhs, mxArray *plhs[],int nrhs, const mxArray *prhs[]) {
  
  //Check number of input arguments
  if(nrhs<3){
    mexErrMsgTxt("Function needs 3 arguments");
  }

  // Check data type of input 
  if (!(mxIsUint8(prhs[0]))) {
    mexErrMsgTxt("First input array must be of type uint8.");
  }

  if (!(mxIsUint8(prhs[1]))) {
    mexErrMsgTxt("Second input array must be of type uint8.");
  }

  if (!(mxIsUint8(prhs[2]))) {
    mexErrMsgTxt("Third input array must be of type uint8.");
  }

  int ndim =  mxGetNumberOfDimensions(prhs[0]);
  const mwSize  *dim_array = mxGetDimensions(prhs[0]);

  //Setup output
  plhs[0]=mxCreateNumericArray(ndim,dim_array,mxDOUBLE_CLASS,mxREAL);
  double* dt = (double *) mxGetPr(plhs[0]); /*basically go from pointers to matlab arrays to pointers to data*/
  plhs[1]=mxCreateNumericArray(ndim,dim_array,mxDOUBLE_CLASS,mxREAL);
  double* segmentation = (double *) mxGetPr(plhs[1]);

  //Setup input
  uint8 *seed_elements, *label_elements,*cost_elements;
  seed_elements = (uint8 *) mxGetPr(prhs[0]); 
  label_elements = (uint8 *) mxGetPr(prhs[1]); 
  cost_elements = (uint8 *) mxGetPr(prhs[2]);


  int x_size=dim_array[1];
  int y_size=dim_array[0];
  int z_size=1;
  if(ndim>2){
    z_size=dim_array[2];
  }

  imagetype seeds = imagetype(seed_elements,x_size,y_size,z_size);
  imagetype labels = imagetype(label_elements,x_size,y_size,z_size);
  imagetype cost = imagetype(cost_elements,x_size, y_size,z_size);

  int nelem = seeds.getNelem();


  //Calculate DT
  MBD_exact(seeds,labels, cost, dt,segmentation);

}
