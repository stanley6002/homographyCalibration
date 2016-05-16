//
//  Jlinkage.h
//  VP detection
//
//  Created by chih-hsiang chang on 1/28/13.
//  Copyright 2013 __MyCompanyName__. All rights reserved.
//

//#include <iostream> 
//#include <stdio.h>
//#include <stdlib.h>
//#include <iomanip>
//#include <math.h>
//# include "Utility.h"



#ifndef JLINKAGE_H
#define JLINKAGE_H


#include <cstdio>
#include <iostream> 
#include <stdio.h>
#include <stdlib.h>
#include <iomanip>
#include <math.h>
#include <vector>
#include "DType .h"
#include "Utility.h"
#include <assert.h>
#include "fit.h"
#include <string.h>
#include <map>
#include "matrix_OP.h"


typedef struct 
{
    std::vector<int>   vData;
    //std::vector<double> vDist;
    double* vDist;
    double minDist;
    int minDistIdx;
    bool*   pBin;
    int pLength;
    bool disable;
}  Cluster;


using namespace std;

extern vector<vector <int> > cluster_result; 
extern std::vector< Cluster > vCluster;
extern std::vector< Cluster > NvCluster;
extern std::vector< vector<int> > New_Cluster;
extern std::vector <pt3> _3D_VP_result;
extern float rot_Matrix[9];
extern bool _1st_frame; 



void Jlinkage();
void Cluster_Initilization();
void Distance_Initilization(int& _row);
static void Compute_Initial_Distance();
static double  getClusterJaccardDist(Cluster c1, Cluster c2);
static void Compute_Initial_Distance(int& row_);
static double getClosestClusters(std::vector<Cluster> &vCluster, int &c1, int &c2);
void intersectiontwoCluster (Cluster &c1, Cluster &c2);
void mergeClusters(std::vector<Cluster> &vCluster, int c1, int c2);
static double  CalculateClusterJaccardDist(int in1 , int in2);
static void ClusterInitialization_Ex(int*list, int NumofInitialCluster);
void   Cluster_Initilization_N(int NumberofCandidacy,int NumbeofTrial, vector<vector<bool> > errorlst);
void   Compute_Initial_Distance_N();
double CalculateClusterJaccardDist_N(int in1 , int in2, vector<Cluster> NvCluster);
double getClosestClusters_N(std::vector<Cluster> &vCluster, int &c1, int &c2);
void   mergeClusters_N(std::vector<Cluster> &vCluster, int c1, int c2);
void   erase_2Dvector();
void test_rot_initial();
void Restore_Rotation_Matrix(pt3* VP1_data/*previous frame*/, pt3* VP2_data  /*current frame*/,  double*R, int size_);
void Generate_Rotation_Matrix(double*V, double*R);


inline void sign_convert_function(pt3 &input)
{
             //if (input.p3<0)
             //{
                 input.p1=-input.p1;
                 input.p2=-input.p2;
                 input.p3=-input.p3;
             //}
}

class connected_VP 
{
public: 
    pt3* vp_point ;
    int size;
    
    
inline void initialize(int _size)
    {
        vp_point= new pt3[_size];
        size =_size;
    }
inline void delete_vp()
    {
        delete [] vp_point;
        size=0;
    }
inline void save_data(vector<pt3>_3D_VP)
    {
        for (int i=0;i<(int)_3D_VP.size();i++)
        {
            vp_point[i]= _3D_VP[i];
        }
    }
inline void print_data()
    {
        for (int i=0;i<size;i++)
        cout<< vp_point[i].p1<<" "<<vp_point[i].p2<<" "<<vp_point[i].p3<<endl;
    
    }
 inline void Find_connected_vp(vector<pt3>_3D_VP, vector <pair<int, int> > &temp) 
    {
        double minimum_distance = 999;
        //vector <pair<int, int> > temp;
        int idx1,idx2;
        
            for (int i=0;i<(int)_3D_VP.size();i++)
            {
                double dis;
               
                for(int j=0;j<size;j++)
                {
                   
                //cout<<i<<" "<<j<<endl;
                //cout<<"incoming "<<_3D_VP[i].p1<<" "<<_3D_VP[i].p2<<" "<<_3D_VP[i].p3<<endl;
                //cout<<"old_pt   "<<vp_point[j].p1<<" "<<vp_point[j].p2<<" "<<vp_point[j].p3<<endl; 
                    
                 dis=((vp_point[j].p1-_3D_VP[i].p1)*(vp_point[j].p1-_3D_VP[i].p1))+((vp_point[j].p2-_3D_VP[i].p2)*(vp_point[j].p2-_3D_VP[i].p2))+((vp_point[j].p3-_3D_VP[i].p3)*(vp_point[j].p3-_3D_VP[i].p3));
                //cout<<dis<<endl;
                if(dis < minimum_distance)
                {
                    idx1= i;
                    idx2= j;
                    minimum_distance= dis;
                }
                    if(dis<0.1)
                    { 
                        temp.push_back(make_pair(idx1,idx2));
                    }
                    minimum_distance=999;
                }
                
            }
    
        }
        
};

  


#endif 