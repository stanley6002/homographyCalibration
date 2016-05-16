//
//  DType .h
//  Homography _calibration
//
//  Created by chih-hsiang chang on 1/30/14.
//  Copyright 2014 __MyCompanyName__. All rights reserved.
//
#ifndef DType_h
#define DType_h

#ifdef __cplusplus
extern "C"
{
#endif 
    //#include "fit.h"
    //    void NLVPoptimization(double *Vangle,point_D* Centerpt,pt3  *Cline);
    
 # define Canny_thd 500
 # define Canny_mask 5
 # define BLACK 0
 # define WHITE 255
 # define line_threshold   3.5
 # define minLength        120
 # define tolerance        5.0     //  line tolerance
 # define segmentation     100
 # define maxima_trial     400     // number of hypothesis
 # define error_deviation  0.0001
 # define error_deviation1 0.0001
 # define Focallength      500
 # define maxima_iteration 400   // number of iteration for fining the j-linkage
    
  // # define deviation 2.0;
    typedef struct  xypoint
    {
        double x;
        double y;
        
    } point;
    typedef struct  Cpoint
    {
        double x;
        double y;
    } point_D;
    
    typedef struct  C1Lpt
    {
        double p1;
        double p2;
        double p3;
    } pt3;
    
    

    
#ifdef __cplusplus
};
#endif

#endif
