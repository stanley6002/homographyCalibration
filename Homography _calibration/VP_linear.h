//
//  VP_linear.h
//  VP detection
//
//  Created by chih-hsiang chang on 2/22/13.
//  Copyright 2013 __MyCompanyName__. All rights reserved.
//


#include "fit.h"
#include "matrix_OP.h"
#include "VP_nonlinear.h"

#include <cstdio>


//void linear_estimation(); 
void linear_estimation(multimap<string, int> &m);
void NL_estimation_driver(double*vVp, int k,pt3 *VP_result);
void outlier_removal( int cluster_k, vector<int>& err_index);
void outliers_screen (double*vVp,int cluster_k);