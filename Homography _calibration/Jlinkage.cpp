

//
//  Jlinkage.cpp
//  VP detection
//
//  Created by chih-hsiang chang on 1/28/13.
//  Copyright 2013 __MyCompanyName__. All rights reserved.
//
#include <cstdio>
#include "Jlinkage.h"
#include "Operation_type.h"  // add Operation_type control


//Edge2 Edg;
//using namespace cluster_list;
//const int _col = maxima_trial;
//const int _row = (int)Edg.size();



std::vector< Cluster > vCluster;
vector<vector<int> > cluster_result; 
std::vector<Cluster> NvCluster;
std::vector< vector<int> > New_Cluster;
bool _1st_frame=true; 
float  rot_Matrix[9];


void Jlinkage()
{
    
    Cluster_Initilization();
    Compute_Initial_Distance();
    
    int c1=0;
    int c2=0;

     multimap<string, int> m;
    
    for(int i=0;i<maxima_iteration;i++)
    {
        //cout<<"iterations i "<<i<<endl;    
        double d= getClosestClusters(vCluster, c1, c2);
        //std::cout<<"distance d"<<" "<<d <<" "<<c1<<" "<<c2<<endl; 
        if (d>=1)
        {
            break;
        //    cout<<"merge_iteration "<<" "<<i<<endl;
        }
        //std::cout<<c1<<" "<<c2<<endl;
        mergeClusters(vCluster, c1,c2);
        //}
    } 
    
    
    for(int j=0;j<vCluster.size();j++)
    {
        if(vCluster[j].vData.size()>0)
        {
           vector<int> cluster_Temp;
           
            for (int i=0;i<(int)vCluster[j].vData.size();i++)
            {
                
                cluster_Temp.push_back(vCluster[j].vData[i]);
                
            }
             cluster_result.push_back(cluster_Temp);
             cluster_Temp.clear();
        }        
    }  
      
    if (typp_test::read()==1)
    {
         linear_estimation(m);
         pair<multimap<string, int>::iterator, multimap<string, int>::iterator> p1;
         p1 = m.equal_range("a");
         vector<int>temp;
        for (multimap<string, int>::iterator it2 = p1.first;it2 != p1.second;++it2)
        {
            //cout << "  [" << (*it2).first << ", " << (*it2).second << "]" << endl;
            temp.push_back((*it2).second);
        }
         New_Cluster.push_back(temp);
         temp.clear();
         //cout<<temp.size()<<endl;
        p1 = m.equal_range("b");
        for (multimap<string, int>::iterator it2 = p1.first;it2 != p1.second;++it2)
        {
            //cout << "  [" << (*it2).first << ", " << (*it2).second << "]" << endl;
            temp.push_back((*it2).second);
        }
        New_Cluster.push_back(temp);
        temp.clear();
        p1 = m.equal_range("c");
        for (multimap<string, int>::iterator it2 = p1.first;it2 != p1.second;++it2)
        {
            //cout << "  [" << (*it2).first << ", " << (*it2).second << "]" << endl;
            temp.push_back((*it2).second);
        }
        New_Cluster.push_back(temp);
        temp.clear();
    }
    
    //   free cluster space here///
    m.clear();
    test_rot_initial();
    
}
void Cluster_Initilization()
{
    int _row  =(int)Edg.size();  // row= number of edges 
    int _col  = maxima_trial; 
    
    //    for(int i=0;i<_col;i++)
    //    {
    //        std::cout<<errorlst[0][i]<<endl;    // col= number trials
    //     }
    // Parameter_Initilization(_row);
    //char buff[10];
    //sprintf(buff, "%d", _col);
    //char*ext =".txt";
    //char filename[100]= "/matrix_print" ;
    //strcat(filename,buff);
    //strcat(filename,ext);
    //FILE *f = fopen("/matrix_print.txt", "w");
    //FILE *f = fopen(filename, "w");
    //fprintf(f, "%s ","["); 
   
    for (int i=0;i<_row;i++)
    {
        Cluster c;
        c.disable = false;
        c.vData.push_back(i);                           //cluster contains only one point
        //c.vDist = (double*) malloc(nPts*sizeof(double));
        c.vDist = new double[_row];
        c.pBin =  new bool[_col];
        //memcpy(c.pBin, totbin+i*nMod, nMod*sizeof(bool) ); //copy point in model space
             
        //std::cout<<endl;    
        for(int inx=0;inx<_col;inx++)
        {
             c.pBin[inx]=  errorlst[inx][i];
            // fprintf(f, "%i ",(int) errorlst[inx][i]);
            //std::cout<<errorlst[inx][i]<<" ";
        }
        // fprintf(f, "%s \n",";");        
        //std::cout<<errorlst[0].size()<<endl;    
        c.pLength = _col;
        vCluster.push_back(c);
    }
  //   fprintf(f, "%s ","]"); 
  //   fclose(f);
    Distance_Initilization(_row);
    ////    std::cout<<endl;
    ////    for (int i=0;i<_col;i++)
    ////    {
    ////        std::cout<<vCluster[0].pBin[i]<<endl;
    ////    
    ////    }
}
static void Compute_Initial_Distance()
{
    //std::cout<<"distance"<<endl;
    for (int i=0; i< (int)vCluster.size(); i++)
        //for (int i=0; i<1; i++)
    { 
        //minimal distance info to speed up computation
        double  minDist    = DBL_MAX;
        int    minDistIdx =  INT_MAX;
        
        for (int j=i+1; j< (int)vCluster.size() ; j++)
        {
            double d = getClusterJaccardDist(vCluster[i], vCluster[j]);
            //std::cout<<j<<endl;
            vCluster[i].vDist[j] = d;
            
            if (d<minDist)
            {
                minDist = d;
                minDistIdx = j;
            }
            //std::cout<< vCluster[i].vDist[j]<<endl;
        }
        //min distance to speed up computation later
        vCluster[i].minDist    = minDist;
        vCluster[i].minDistIdx = minDistIdx;
    }
}
static double getClusterJaccardDist(Cluster c1, Cluster c2)
{
    double p=0;
    double s=0;
    
    bool* b1 = c1.pBin;
    bool* b2 = c2.pBin;
    
    for (int i = 0; i < c1.pLength; i++)
    {
        //Jlinkage
        p+= (double) (b1[i] & b2[i]);
        s+= (double) (b1[i] | b2[i]);
    }
    
    if (s>0)
        p/= s;
    else
        p = 0;
    
    return (1.0 - p);  
}
static double getClosestClusters(std::vector<Cluster> &vCluster, int &c1, int &c2)
{
    //#ifdef DEBUG
    // MPRINTF("getting closest clusters (%d)...", vCluster.size() );
    //int nbDisable =0;
    //for (int i=0; i < vCluster.size(); i++)
    //     if (vCluster[i].disable) nbDisable++;
    // MPRINTF("nb disable= %d\n", nbDisable);
    //#endif
    double minDist = DBL_MAX;
    
    
    for (int i=0; i < vCluster.size(); i++)
    {
        Cluster c = vCluster[i];
        //cout<<i<<endl;
        if (c.disable) 
        {
            continue;
        }
        
        
        if (c.minDist >= minDist) 
        {
            continue;
        } //nothing closer to that cluster, skipping check
        
        minDist = c.minDist;
        c1 = i;
        c2 = c.minDistIdx;
        //std::cout<<c1<<" "<<c2<<" "<<endl;
        //below is useless
        // 	{
        // 	  //MPRINTF("\t(%d) %lf\n", j, c.vDist[j]);
        // 	  if (c.vDist[j] < minDist)
        // 	    {
        // 	      c1 = i;
        // 	      c2 = j;
        // 	      minDist = c.vDist[j];
        // 	    }	  
        // 	}
    }
    
    assert(c1<c2);
    
    
    //#ifdef DEBUG
    //    printf("%d %d -> %lf\n", c1,c2, minDist);
    //#endif
    return minDist;
    
}

void Distance_Initilization(int & row_)
{
    for (int i=0; i< row_; i++)
    {
        for (int j=0; j< row_; j++)
        {
            vCluster[i].vDist[j] = DBL_MAX;
        }
    }
}
void intersectiontwoCluster (Cluster &c1, Cluster &c2)
{
    
    for (int j = 0; j < c1.pLength; j++)
    {
        c1.pBin[j] &= c2.pBin[j]; 
    }
    
}
void mergeClusters(std::vector<Cluster> &vCluster, int c1, int c2)
{
    //#ifdef DEBUG
    //    MPRINTF("Merging %d %d\n", c1,c2);
    //#endif
    //    
    //#ifdef DEBUG
    //    assert(c1<c2);
    //    
    //    if(vCluster[c2].disable )
    //        mexErrMsgIdAndTxt("stats:linkagemex:error",
    //                          "already disable");
    //    if(vCluster[c1].disable )
    //        mexErrMsgIdAndTxt("stats:linkagemex:error",
    //                          "Should not be disable");
    //#endif
    
    //merging:
    //insert data of c2 into c1
    vCluster[c1].vData.insert(vCluster[c1].vData.end(), 
                              vCluster[c2].vData.begin(), 
                              vCluster[c2].vData.end()  );
    
    // for(int i=0;i<(int)vCluster[c1].vData.size();i++)
    // {
    //     cout<< vCluster[c1].vData[i]<<endl;
    // }
    // cout<<endl;
    
    //erase c2's data and disable this cluster
    //we don't remove from vCluster for efficiency
    vCluster[c2].vData.clear();
    vCluster[c2].disable = true;
    
    //#ifdef DEBUG
    //    MPRINTF("Cluster %d contains %d point\n", c1, 
    //            vCluster[c1].vData.size() );
    //#endif
    
    //Compute new value of c1
    intersectiontwoCluster( vCluster[c1],  vCluster[c2] );
    
    //min Dist of c1 is now invalid, recompute below
    vCluster[c1].minDist    = DBL_MAX;
    vCluster[c1].minDistIdx = INT_MAX;
    
    //recompute distance of all cluster to c1
    for (int i=0; i< vCluster.size(); i++)
    {
        //clear distance for removed (merged into) cluster
        vCluster[i].vDist[c2] = DBL_MAX;
        vCluster[c2].vDist[i] = DBL_MAX;
        
        if (i==c1) continue;
        if (vCluster[i].disable) continue;
        
        
        //distance is saved in "upper diagonal"
        int imin = std::min(i,c1);
        int imax = std::max(i,c1);
        
        //get distance
        double d = getClusterJaccardDist(vCluster[imin], vCluster[imax]);
        vCluster[imin].vDist[imax] = d;
        
        //save as min distance if it smaller than the current one
        if (d< vCluster[imin].minDist)
      	{
            vCluster[imin].minDist = d;
            vCluster[imin].minDistIdx = imax;
      	}
    }
    
    //clusters with minimal distance to c2 and c1, min dist must be reinitialized
    for (int i=0; i< vCluster.size(); i++)
    {
        if (vCluster[i].minDistIdx != c2 &&
            vCluster[i].minDistIdx != c1) continue; //nothing to do
        
        //recompute minimal distance
        vCluster[i].minDist    =  DBL_MAX;
        for (int j=i+1;j<vCluster.size(); j++)
        {
            double d = vCluster[i].vDist[j];// = d;
            if (d< vCluster[i].minDist)
            {
                vCluster[i].minDist = d;
                vCluster[i].minDistIdx = j;
            }
        }
        
    }
}
void Cluster_Initilization_N(int NumberofCandidacy,int NumbeofTrial, vector<vector<bool> > errorlst)
{
    
    int _row  = NumberofCandidacy;  // row= number of edges 
    int _col  = NumbeofTrial; 
   
    char buff[10];
    sprintf(buff, "%d", _col);
    char*ext =".txt";
    char filename[100]= "/matrix_print" ;
    strcat(filename,buff);
    strcat(filename,ext);
    //FILE *f = fopen("/matrix_print.txt", "w");
    FILE *f = fopen(filename, "w");
    fprintf(f, "%s ","["); 

    
    for (int i=0;i<_row;i++)
    {
        Cluster c;
        c.disable = false;
        c.vData.push_back(i);          //cluster contains only one point
        //c.vDist = (double*) malloc(nPts*sizeof(double));
        c.vDist = new double[_row];
        c.pBin =  new bool[_col];
        //   memcpy(c.pBin, totbin+i*nMod, nMod*sizeof(bool) ); //copy point in model space
        //   std::cout<<endl;    
        for(int inx=0;inx<_col;inx++)
        {
            c.pBin[inx]=  errorlst[inx][i];
             //    fprintf(f, "%i ",(int) errorlst[inx][i]);
            //cout<<errorlst[inx][i]<<" ";
        }
         // fprintf(f, "%s \n",";");        
        //cout<<endl;
        c.pLength = _col;
        NvCluster.push_back(c);
    }
    //fprintf(f, "%s ","]"); 
    fclose(f);
    
    
    for (int i=0; i< _row; i++)
    {
        for (int j=0; j< _row; j++)
        {
            
            NvCluster[i].vDist[j] = DBL_MAX;
        }
    }
}
void Compute_Initial_Distance_N()
{
    for (int i=0; i< (int)NvCluster.size(); i++)
        
    { 
        double  minDist    = DBL_MAX;
        int     minDistIdx =  INT_MAX;
        
        for (int j=i+1; j< (int)NvCluster.size() ; j++)
        {
            double d = getClusterJaccardDist(NvCluster[i], NvCluster[j]);
            // std::cout<<"i= "<<i<<" j= "<<j<<" Initial_Distance_ "<<d<<endl;
            NvCluster[i].vDist[j] = d; 
            if (d<minDist)
            {
                minDist = d;
                minDistIdx = j;
            }
        }
        NvCluster[i].minDist    = minDist;
        NvCluster[i].minDistIdx = minDistIdx;
    }
}
double CalculateClusterJaccardDist_N(int in1 , int in2, vector<Cluster> NvCluster)
{
    
    Cluster c1 = NvCluster[in1];
    Cluster c2 = NvCluster[in2];
    
    double p=0;
    double s=0;
    
    bool* b1 = c1.pBin;
    bool* b2 = c2.pBin;
    
    for (int i = 0; i < c1.pLength; i++)
    {
        //Jlinkage
        p+= (double) (b1[i] & b2[i]);
        s+= (double) (b1[i] | b2[i]);
    }
    
    if (s>0)
        p/= s;
    else
        p = 0;
    
    return (1.0 - p);  
}
double getClosestClusters_N(std::vector<Cluster> &NvCluster, int &c1, int &c2)
{
    double minDist = DBL_MAX;    
    for (int i=0; i < NvCluster.size(); i++)
    {
        Cluster c = NvCluster[i];
        if (c.disable) 
        {
            continue;
        }
        if (c.minDist >= minDist) 
        {
            continue;
        } 
        minDist = c.minDist;
        c1 = i;
        c2 = c.minDistIdx;
    }
    
    assert(c1<c2);
    return minDist;
}

void mergeClusters_N(std::vector<Cluster> &NvCluster, int c1, int c2)
{
    NvCluster[c1].vData.insert(NvCluster[c1].vData.end(), 
                               NvCluster[c2].vData.begin(), 
                               NvCluster[c2].vData.end()  );
    
    //erase c2's data and disable this cluster
    //we don't remove from vCluster for efficiency
    NvCluster[c2].vData.clear();
    NvCluster[c2].disable = true;
    
    //Compute new value of c1
    intersectiontwoCluster( NvCluster[c1],  NvCluster[c2] );
    
    //min Dist of c1 is now invalid, recompute below
    NvCluster[c1].minDist    = DBL_MAX;
    NvCluster[c1].minDistIdx = INT_MAX;
    
    //recompute distance of all cluster to c1
    for (int i=0; i< NvCluster.size(); i++)
    {
        //clear distance for removed (merged into) cluster
        NvCluster[i].vDist[c2] = DBL_MAX;
        NvCluster[c2].vDist[i] = DBL_MAX;
        
        if (i==c1) continue;
        if (NvCluster[i].disable) continue;
        
        
        //distance is saved in "upper diagonal"
        int imin = std::min(i,c1);
        int imax = std::max(i,c1);
        
        //get distance
        double d = getClusterJaccardDist(NvCluster[imin], NvCluster[imax]);
        NvCluster[imin].vDist[imax] = d;
        
        //save as min distance if it smaller than the current one
        if (d< NvCluster[imin].minDist)
      	{
            NvCluster[imin].minDist = d;
            NvCluster[imin].minDistIdx = imax;
      	}
    }
    
    //clusters with minimal distance to c2 and c1, min dist must be reinitialized
    for (int i=0; i< NvCluster.size(); i++)
    {
        if (NvCluster[i].minDistIdx != c2 &&
            NvCluster[i].minDistIdx != c1) continue; //nothing to do
        
        //recompute minimal distance
        NvCluster[i].minDist    =  DBL_MAX;
        for (int j=i+1;j<NvCluster.size(); j++)
        {
            double d = NvCluster[i].vDist[j];// = d;
            if (d< NvCluster[i].minDist)
            {
                NvCluster[i].minDist = d;
                NvCluster[i].minDistIdx = j;
            }
        }
        
    }
}
void erase_2Dvector()
{

       while (!vCluster.empty())
        {
            
            vCluster.erase(vCluster.begin());
            
        }
                
        while (!NvCluster.empty())
        {
            
            NvCluster.erase(NvCluster.begin());
            
        }
        
    while (!cluster_result.empty())
    {
        int index = (int) cluster_result.size()-1;
        
       
        
        while (!cluster_result[index].empty())
        {
            
            cluster_result[index].erase(cluster_result[index].begin());
            
        }
         cluster_result.erase(cluster_result.begin());
    }
    while (!New_Cluster.empty())
    {
        int index = (int) New_Cluster.size()-1;
        
        
        
        while (!New_Cluster[index].empty())
        {
            
            New_Cluster[index].erase(New_Cluster[index].begin());
            
        }
        New_Cluster.erase(New_Cluster.begin());
    }
    while (!_3D_VP_result.empty())
    {
        
        _3D_VP_result.erase(_3D_VP_result.begin());
        
    }

   // cout << cluster_result.size()<<endl;

}
connected_VP vppts;
void test_rot_initial()
{
   
   int _size= (int) _3D_VP_result.size();
    vector <pair<int, int> > temp;
    for (int i=0;i<_size;i++)
    {
        if (_3D_VP_result[i].p3<0)
        {
            _3D_VP_result[i].p1=-_3D_VP_result[i].p1;
            _3D_VP_result[i].p2=-_3D_VP_result[i].p2;
            _3D_VP_result[i].p3=-_3D_VP_result[i].p3;
        }
        
       //cout<< "Vp_sign_convert "<< _3D_VP_result[i].p1<<" "<<_3D_VP_result[i].p2<<" "<<_3D_VP_result[i].p3<<endl; 
    }
   
   if (_1st_frame==true)
   {
      
       vppts.initialize(_size);
       vppts.save_data(_3D_VP_result); 
       vppts.print_data();
       //vppts.delete_vp();        
   }
    if(_1st_frame==false)
   {
       if ((int) _3D_VP_result.size()>=2)
        {
         vppts.Find_connected_vp(_3D_VP_result,temp);
         
         //cout<<"incoming_ VPsize " <<(int) _3D_VP_result.size()<<endl;
         //cout<<"sizeoftempmatching_ "<<(int) temp.size()<<endl;
         
           if(temp.size()>1)
           {
           
           update_value= true; 
               
           double R[9];  //rotation matrix; 
           //double RT[9];
           pt3* current = new pt3[(int) temp.size()];   // temp(current, previous)
           pt3* previous = new pt3[(int) temp.size()];
         
            for (int i=0;i<(int) temp.size();i++)
            {
              
              current[i]= _3D_VP_result[temp[i].first];
              previous[i]= vppts.vp_point[temp[i].second];
                //cout<<"current " <<current[i].p1<<" "<<current[i].p2<<" "<<current[i].p3<<endl;
                //cout<<"previous "<<previous[i].p1<<" "<<previous[i].p2<<" "<<previous[i].p3<<endl;
            }
               
               Restore_Rotation_Matrix(previous,current,R,(int) temp.size());
               //cout<<"Rotation_matrix"<<endl;
                matrix_print(3, 3, R);
               //matrix_transpose(3, 3, R, RT);
               
               for(int i=0;i<9;i++)
                   rot_Matrix[i]= (float)R[i];
               
               delete [] current; delete [] previous;
           }
                    vppts.delete_vp();  
        vppts.initialize(_size);
        vppts.save_data(_3D_VP_result);    
        //cout<<endl;
        }
       if(_3D_VP_result.size()<=1)
       {
           
           update_value= false; 
           
       }

     }

     //_1st_frame=false;
        

}

void Restore_Rotation_Matrix(pt3* VP1_data/*previous frame*/, pt3* VP2_data  /*current frame*/,  double*R, int size_)
{
    double* A_matrix= new double[16];
    double A_temp_matrix[16]={0};
     
    
    for(int i=0;i<size_;i++)
    {
        A_matrix[0]=0;
        A_matrix[1]=VP2_data[i].p1 - VP1_data[i].p1;
        A_matrix[2]=VP2_data[i].p2 - VP1_data[i].p2;
        A_matrix[3]=VP2_data[i].p3 - VP1_data[i].p3;
        A_matrix[4]=VP1_data[i].p1 - VP2_data[i].p1;
        A_matrix[5]=0;
        A_matrix[6]=-VP1_data[i].p3 - VP2_data[i].p3;
        A_matrix[7]= VP2_data[i].p2 + VP1_data[i].p2;
        A_matrix[8]= VP1_data[i].p2 - VP2_data[i].p2;
        A_matrix[9]= VP2_data[i].p3 + VP1_data[i].p3;
        A_matrix[10]=0;
        A_matrix[11]=-VP1_data[i].p1 - VP2_data[i].p1;
        A_matrix[12]= VP1_data[i].p3 - VP2_data[i].p3;
        A_matrix[13]= -VP1_data[i].p2- VP2_data[i].p2;
        A_matrix[14]= VP2_data[i].p1+VP1_data[i].p1; 
        A_matrix[15]=0;
        
        double AT_matrix[16];
        double ATA[16];
        
        matrix_transpose(4,4,A_matrix,AT_matrix);
        matrix_product(4,4,4,4,AT_matrix,A_matrix,ATA);
        for(int i=0;i<16;i++)
            A_temp_matrix[i]=A_temp_matrix[i]+ATA[i];

    }
    double *U_= new double[16];
    double *S=  new double[16];
    double *VT= new double[16];
    //double *V = new double[16]; 
    double *V_vector= new double[4];
    //double *R1 = new double [9];
    dgesvd_driver(4,4,A_temp_matrix,U_,S,VT);
    
    //matrix_print(4,4,VT);
    //matrix_transpose(4,4,VT,V);
    //matrix_print(4,4,VT);
    
    V_vector[0]= VT[12];V_vector[1]= VT[13];V_vector[2]= VT[14];V_vector[3]= VT[15];
    //matrix_print(4,1,V_vector);
    Generate_Rotation_Matrix(V_vector,R);
    //cout<<"Rotation_matrix_ "<<endl;
    //matrix_print(3,3,R1);
  
    //for(int i=0;i<16;i++)
         //cout<<A_temp_matrix[i]<<" "<<endl; 
    delete [] A_matrix;delete [] U_;delete [] S; delete [] VT;
    delete [] V_vector; //delete [] R1;
}

void Generate_Rotation_Matrix(double*q, double* R)
{
      
  R[0] = (q[0]*q[0])+(q[1]*q[1])-(q[2]*q[2])-(q[3]*q[3]);
  R[4] = (q[0]*q[0])-(q[1]*q[1])+(q[2]*q[2])-(q[3]*q[3]);
  R[8] = (q[0]*q[0])-(q[1]*q[1])-(q[2]*q[2])+(q[3]*q[3]);
  //cout<<R[0]<<" "<<R[4]<<" "<<R[8];
  R[1]= 2*((q[1]*q[2])-(q[0]*q[3]));
  R[2]= 2*((q[1]*q[3])+(q[0]*q[2]));
  R[3]= 2*((q[1]*q[2])+(q[0]*q[3]));
  R[5]= 2*((q[2]*q[3])-(q[0]*q[1]));
  R[6]= 2*((q[1]*q[3])-(q[0]*q[2]));
  R[7]= 2*((q[2]*q[3])+(q[0]*q[1]));

    
}