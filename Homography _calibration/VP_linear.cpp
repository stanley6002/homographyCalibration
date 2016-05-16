 //
//  VP_linear.cpp
//  VP detection
//
//  Created by chih-hsiang chang on 2/22/13.
//  Copyright 2013 __MyCompanyName__. All rights reserved.
//
#include "VP_linear.h"
#include "Jlinkage.h"
#include <cstdio>

# define MAXIMUM_ERROR 0.05
std::vector<pt3> _3D_VP_result;   //Global_variable : result of vanishing_points

void linear_estimation(multimap<string, int> &m)
{
    
 
    int Num_cluster = (int)cluster_result.size(); 
    //std::cout<<"Number of Cluster "<<Num_cluster<<endl;
    double* mAt = new double [9];
    double weight=1.0;
    int location=0;
    double *eig_value= new double[3];
    double *vVP= new double [3];
    pt3 *VP_result= new pt3[1];
    
    vector<int> shift_key;
    int index_shift_key =0;

    // temp data  
    vector<vector <int> > temp;
    
    // Initialize : 
    

for(int cluster_k=0;cluster_k<Num_cluster;cluster_k++)
//    for(int cluster_k=2;cluster_k<3;cluster_k++)
    {
        //std::cout<<k<<" Kth_cluster "<<endl;
        location =(int)cluster_result[cluster_k].size();
  if (location>=11)
    {  

    for (int j=0;j<location;j++)
    {
        int index = cluster_result[cluster_k][j];
         //std::cout<<index<<" ";
         //Edg.Print_VL(index);
         //std::cout<<endl;
        for (int i=0;i<9;i++)
        {
        
            mAt[i]+= weight *Edg.VLt_read(index,i);
           
        }
        //matrix_print(3, 3, mAt);
    }
        //;
        dsyev_driver(3, mAt, 3,eig_value);  // eigen decomposition 
        //matrix_print(3, 3, mAt);
        eig_value[0] =fabs(eig_value[0]);
        eig_value[1] =fabs(eig_value[1]);
        eig_value[2] =fabs(eig_value[2]);
        //std::cout<<eig_value[0]<<" "<<eig_value[1]<<" "<<eig_value[2]<<endl;
        if (eig_value[0] < eig_value[1])
        {
            if (eig_value[0] < eig_value[2])
            {
                vVP[0] = mAt[0];
                vVP[1] = mAt[1];
                vVP[2] = mAt[2];
            }
            else
            {
                vVP[0] = mAt[6];
                vVP[1] = mAt[7];
                vVP[2] = mAt[8];
            }
        }
        else
        {
            if (eig_value[1] < eig_value[2])
            {
                vVP[0] = mAt[3];
                vVP[1] = mAt[4];
                vVP[2] = mAt[5];
            }
            else
            {
                vVP[0] = mAt[6];
                vVP[1] = mAt[7];
                vVP[2] = mAt[8];
            }
        }
        // std::cout<<"Cluster_size "<<cluster_result[4].size()<<endl;
        
         outliers_screen (vVP,cluster_k);
         NL_estimation_driver(vVP,cluster_k,VP_result);
        
//         if (VP_result->p3<0)
//         {
//             VP_result->p1=-VP_result->p1;
//             VP_result->p2=-VP_result->p2;
//             VP_result->p3=-VP_result->p3;
//         }
        
         cout<<"VP_result_3D "<<VP_result->p1<<" "<<VP_result->p2<<" "<<VP_result->p3<<endl;
         //cout<<"Vp_result_2D  "<<(VP_result->p1/VP_result->p3)<<" "<<(VP_result->p2/VP_result->p3)<<endl;
         // temprary save pt3 for transferring data 

        pt3 temp;
        
        if((VP_result->p1/VP_result->p3)>0 )   // color R
        {
           if(abs(VP_result->p2)<0.7)
           {
            string string1 = "a";   
            for (int i=0;i< location;i++)
            {m.insert(pair<string, int>(string1, cluster_result[cluster_k][i]));}
            
             temp.p1=VP_result->p1;temp.p2=VP_result->p2;temp.p3= VP_result->p3;
            
             _3D_VP_result.push_back(temp);
             cout<<"color_R"<<endl;
           }
        }
        if((VP_result->p1/VP_result->p3)<0 )    // color G
        {
            if(abs(VP_result->p2)<0.8)
            {
             string string1 = "b";
             for (int i=0;i< location;i++)
             {m.insert(pair<string, int>(string1, cluster_result[cluster_k][i]));}  
            
             temp.p1=VP_result->p1;temp.p2=VP_result->p2;temp.p3= VP_result->p3;
            
             _3D_VP_result.push_back(temp);
             cout<<"color_G"<<endl;   
            }
        }
        //if((fabs(VP_result->p2/VP_result->p1))>4)  // color B
        //{
             if (fabs(VP_result->p2)>0.8)
             {
             string string1 ="c";
             for (int i=0;i< location;i++)
             {
             m.insert(pair<string, int>(string1, cluster_result[cluster_k][i])); }
             
             temp.p1=VP_result->p1;temp.p2=VP_result->p2;temp.p3= VP_result->p3;
             
             _3D_VP_result.push_back(temp);
             cout<<"color_B"<<endl;
                }
        //}  
    }  
 
    else 
     {
       
         // save shift key //
         shift_key.push_back(cluster_k);
         continue;
      }  
       
         
}
    cout<<endl; 
    //cout<< shift_key.size()<<endl;
   
    int s_index=0;
    
    for (int i=0;i<(int)shift_key.size();i++)
    {
        int shift_index = shift_key[i];
        shift_index-=s_index;
       
        cluster_result.erase(cluster_result.begin()+shift_index);
        //while (!vc1[index_delete].empty())
        //{
        // int index = (int) vc1[index_delete].size()-1;
        while (!cluster_result[shift_index].empty())
        {
            
            cluster_result[shift_index].erase(cluster_result[shift_index].begin());
        } 
       
         s_index++;
    }
    
    //cout<<""<<endl;
    
   
    delete mAt, delete eig_value, delete vVP;
    delete VP_result;
    
}
void  outliers_screen (double* vVp,int cluster_k)
{
    
    //cout<<vVp[0]<<" "<<vVp[1]<<" "<<vVp[2]<<endl;
    
    int NumEdg= (int)cluster_result[cluster_k].size();
    point_D*Centerpt= new point_D[NumEdg];
    pt3    *Cline = new pt3[NumEdg];
    double* err= new double[3] ;
    double error=0;
    
    for(int i=0;i<NumEdg;i++)
    { 
        int index= cluster_result[cluster_k][i];
        Centerpt[i]=Edg.GetCpoint(index);
        Cline[i] = Edg.GetC1Lpt(index);
    }
    
    vector<int> err_index;
    for (int i=0;i<NumEdg;i++)
    {
        
        err[0]= Cline[i].p1*vVp[0];
        err[1]= Cline[i].p2*vVp[1];
        err[2]= Cline[i].p3*vVp[2];
        error= sqrt((err[0]*err[0])+ (err[1]*err[1])+ (err[2]*err[2]));
        //cout<<error<<endl;
        //outlier_removal(error, cluster_k, i);             
        if (error> MAXIMUM_ERROR)
        {
            err_index.push_back(i);              
        }
    }
      outlier_removal(cluster_k, err_index);

    delete  Cline;
    delete  Centerpt;
    delete  err;
}  
 void NL_estimation_driver(double *vVP,int cluster_k, pt3* VP_result)
{
    int NumEdg= (int)cluster_result[cluster_k].size();
   
    point_D* pt= new point_D[NumEdg];
    pt3    * line = new pt3[NumEdg];
        
        for(int i=0;i<NumEdg;i++)
        { 
            int index= cluster_result[cluster_k][i];
            pt[i]=Edg.GetCpoint(index);
            line[i] = Edg.GetC1Lpt(index);
        }
        double  vAngle[2];
        vAngle[0] = atan2(vVP[1], vVP[0]);
        vAngle[1] = acos(vVP[2]);
        NLVPoptimization(NumEdg,vAngle, pt, line,VP_result);
   
}
void outlier_removal(int cluster_k, vector<int>& err_index)
{
    int NumEdg= (int)err_index.size(); 
    int j =0;         // shift key for delete //
  
    for (int i=0;i<NumEdg;i++)
    {
    
    int index = err_index[i];
    index -=j;       // key shift //
    cluster_result[cluster_k].erase(cluster_result[cluster_k].begin()+index);
    j++;
    
    } 

}

