//
//  Utility.cpp
//  Homography _calibration
//
//  Created by chih-hsiang chang on 1/28/14.
//  Copyright 2014 __MyCompanyName__. All rights reserved.
//

#include "Utility.h"
#include "matrix_OP.h"
#include <ctime>
#include <time.h>
#include <sys/time.h>
#include <stdio.h>
#ifdef __MACH__
#include <mach/clock.h>
#include <mach/mach.h>
#endif
#include <string>
double elapsed =0.0;

void current_utc_time(struct timespec *ts) {
    
#ifdef __MACH__ // OS X does not have clock_gettime, use clock_get_time
    clock_serv_t cclock;
    mach_timespec_t mts;
    host_get_clock_service(mach_host_self(), CALENDAR_CLOCK, &cclock);
    clock_get_time(cclock, &mts);
    mach_port_deallocate(mach_task_self(), cclock);
    ts->tv_sec = mts.tv_sec;
    ts->tv_nsec = mts.tv_nsec;
#else
    clock_gettime(CLOCK_REALTIME, ts);
#endif
    
}


 CvCapture *camCapture;
 CvCapture* capture;
 IplImage*  frame ;
 int Img_width;
 int Img_height;
 bool update_value= false; 
 float _matrix[9];
 bool rot_update= false;
 float Matrix[16];

void idle()
{
     //capture next frame from next capture frame
     // use skipFrame for image resize;
     frame = scaleFrame(capture,0);
    
}
void idle1()
{
    //capture next frame from next capture frame
    
    frame = cvQueryFrame(camCapture);  
//frame = skipNFrames(camCapture, 1);
}

void display()
{
    glClear( GL_COLOR_BUFFER_BIT );
    
    Jlinkage_app();
         
    cv::Mat out;
    cv::Mat tempimage(frame);
    
    std::ostringstream os;
    os << (1/elapsed);
    string temp = "Frame rate: ";
    string txt= temp + os.str();
    cv::Point Pt(10,50);
    int linetype=2;
    cv::putText(tempimage,txt,Pt,CV_FONT_HERSHEY_COMPLEX,0.8,cv::Scalar(0,0,0),2,linetype);
    
    cv::flip (tempimage,out,0);
    

    //cvShowImage("rt", frame);
    glDrawPixels( out.size().width, out.size().height,GL_BGR, GL_UNSIGNED_BYTE,out.ptr());    
    
    //////////////////////////////////////////////////////////////////////////////////
    // Here, set up new parameters to render a scene viewed from the camera.
    
    //set viewport
    glViewport(0, 0, Img_width, Img_height);
    
    //set projection matrix using intrinsic camera params
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    //gluPerspective is arbitrarily set, you will have to determine these values based
    //on the intrinsic camera parameters
    gluPerspective(40, Img_width*1.0/Img_height, 1, 20); 
    
    
    
    
    //you will have to set modelview matrix using extrinsic camera params
    glMatrixMode(GL_MODELVIEW);
    //glLoadIdentity();
    
   
    float _matrix_temp [9];
    float _temp [9];
    
    if (_1st_frame==true)
      {
           
              _matrix[0]=1.0;       _matrix[1]=0.0;    _matrix[2]=0.0; 
              _matrix[3]=0.0;       _matrix[4]=1.0;    _matrix[5]=0.0;
              _matrix[6]=0.0;       _matrix[7]=0.0;    _matrix[8]=1.0;
              
              Matrix[0] = _matrix[0];      Matrix[1] = _matrix[1];     Matrix[2] = _matrix[2];    Matrix[3] = 0.0;
              Matrix[4] = _matrix[3];      Matrix[5] = _matrix[4];     Matrix[6] = _matrix[5];    Matrix[7] = 0.0;
              Matrix[8] = _matrix[6];      Matrix[9] = _matrix[7];     Matrix[10]= _matrix[8];    Matrix[11]= 0.0;
              Matrix[12]= 0.0;             Matrix[13]= 0.0;            Matrix[14]= -2.0;          Matrix[15]= 1;
              glLoadMatrixf(Matrix);
              _1st_frame=false;
          
        }
    
    if (update_value==false)
       { 
//        if (rot_update==true)
//        {
//        _matrix[0]=1.0;       _matrix[1]=0.0;    _matrix[2]=0.0; 
//        _matrix[3]=0.0;       _matrix[4]=1.0;    _matrix[5]=0.0;
//        _matrix[6]=0.0;       _matrix[7]=0.0;    _matrix[8]=1.0;
        
//        Matrix[0] = _matrix[0];    Matrix[1] = _matrix[1];     Matrix[2] = _matrix[2];    Matrix[3] = 0.0;
//        Matrix[4] = _matrix[3];    Matrix[5] = _matrix[4];     Matrix[6] = _matrix[5];    Matrix[7] = 0.0;
//        Matrix[8] = _matrix[6];    Matrix[9] = _matrix[7];     Matrix[10]= _matrix[8];    Matrix[11]= 0.0;
//        Matrix[12]= 0.0;           Matrix[13]= 0.0;            Matrix[14]= -2.0;          Matrix[15]= 1;
//        glLoadMatrixf(Matrix);
//        }
        
//        if (update_value==false)
//        {
//            _matrix[0]=1.0;       _matrix[1]=0.0;    _matrix[2]=0.0; 
//            _matrix[3]=0.0;       _matrix[4]=1.0;    _matrix[5]=0.0;
//            _matrix[6]=0.0;       _matrix[7]=0.0;    _matrix[8]=1.0;
//            //matrix_product_f(3, 3, 3, 3, _matrix, rot_Matrix, _temp);
//            
//            Matrix[0] = _matrix[0];      Matrix[1] = _matrix[1];     Matrix[2] = _matrix[2];    Matrix[3] = 0.0;
//            Matrix[4] = _matrix[3];      Matrix[5] = _matrix[4];     Matrix[6] = _matrix[5];    Matrix[7] = 0.0;
//            Matrix[8] = _matrix[6];      Matrix[9] = _matrix[7];     Matrix[10]= _matrix[8];    Matrix[11]= 0.0;
//            Matrix[12]= 0.0;             Matrix[13]= 0.0;            Matrix[14]= -2.0;          Matrix[15]= 1;
//            glLoadMatrixf(Matrix);
//        }
//        rot_update=false;
     }
    if (update_value==true)
     {    
     
      matrix_product_f(3, 3, 3, 3, _matrix, rot_Matrix, _temp);
      float _temp1[9]; 
         
            // _temp1[0]= _temp[0]; _temp1[1]=  -_temp[1]; _temp1[2]= -_temp[2];
            // _temp1[3]= -_temp[3]; _temp1[4]= _temp[4];  _temp1[5]= _temp[5];
            // _temp1[6]= -_temp[6]; _temp1[7]= -_temp[7]; _temp1[8]= _temp[8];
         
                  _temp1[0]=  _temp[0]; _temp1[1]= _temp[1];  _temp1[2]= _temp[2];
                  _temp1[3]=  _temp[3]; _temp1[4]= _temp[4];  _temp1[5]= _temp[5];
                  _temp1[6]=  _temp[6]; _temp1[7]= _temp[7];  _temp1[8]= _temp[8];
         
      Matrix[0] = _temp1[0];  Matrix[1] = _temp1[3];     Matrix[2]=    _temp1[6];    Matrix[3] = 0.0;
      Matrix[4] = _temp1[1];  Matrix[5] = _temp1[4];     Matrix[6] =   _temp1[7];    Matrix[7] = 0.0;
      Matrix[8] = _temp1[2];  Matrix[9] = _temp1[5];     Matrix[10]=  _temp1[8];    Matrix[11]= 0.0;
      Matrix[12]= 0.0;        Matrix[13]= 0.0;          Matrix[14]= -2.0;       Matrix[15]= 1;
         
         
         for(int i=0;i<9;i++)
         {
          _matrix[i]=_temp[i];     
         // rot_update=true;    
         }
   //    glLoadMatrixf(Matrix);
    
     }
 //   }
    
    glLoadMatrixf(Matrix);  
    //gluLookAt(0, 0, 3, 0, 0, 0, 0, 1, 0);  
    
    
    /////////////////////////////////////////////////////////////////////////////////
    // Drawing routine
    
    // now that the camera params have been set, draw your 3D shapes
    // first, save the current matrix
    glPushMatrix();
    //move to the position where you want the 3D object to go
    glTranslatef(0, 0, 0); 
    //this is an arbitrary position for demonstration
    //you will need to adjust your transformations to match the positions where
    //you want to draw your objects(i.e. chessboard center, chessboard corners)
    //glColor3f(1.0,1.0,1.0);
    // glutSolidCube(0.3);
    //glutSolidTeapot(0.3);
    //glutWireCube(0.3);
    //glutSolidSphere(.3, 100, 100)
    
    glEnable(GL_TEXTURE_2D);
    DrawCone ();
    glDisable(GL_TEXTURE_2D);
    glFlush();
    drawAxes(0.5);
    
    //glPushMatrix();
    
    glPopMatrix();
    // show the rendering on the screen
    glutSwapBuffers();
    
    // post the next redisplay
    glutPostRedisplay();
    out.release();
    tempimage.release();
}
void drawAxes(float length)
{
   
    //glPushAttrib(GL_POLYGON_BIT | GL_ENABLE_BIT | GL_COLOR_BUFFER_BIT) ;
    
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE) ;
    //glDisable(GL_LIGHTING) ;
    
    glBegin(GL_LINES) ;
    glColor3f(1,0,0) ;
    glVertex3f(0,0,0) ;
    glVertex3f(length,0,0);
    
    glColor3f(0,0,1) ;
    glVertex3f(0,0,0) ;
    glVertex3f(0,length,0);
    
    glColor3f(0,1,0) ;
    glVertex3f(0,0,0) ;
    glVertex3f(0,0,length);
    glEnd() ;
    //glPopAttrib() ;
 
}
void reshape( int w, int h )
{
    // set OpenGL viewport (drawable area)
    glViewport( 0, 0, w, h );
}
IplImage* scaleFrame(CvCapture* capture, int n)
{

    IplImage* temp;
    temp = cvQueryFrame(capture);
    
    IplImage* tempframe = cvCreateImage ( cvSize(scaled_width,scaled_height), temp->depth, 3);
    cvResize(temp, tempframe);

    return tempframe;
}
void display1()
{
    // clear the window
    glClear( GL_COLOR_BUFFER_BIT );
    
    //  show the current camera frame
    //  based on the way cv::Mat stores data, you need to flip it before displaying it
    
    cv::Mat out1;
    //cout<<frame->height<<" "<<endl;
    cv::Mat tempimage1(frame);
    
    //cvShowImage("test",frame);
    //cv::Mat tempimage1(out1);
    cv::flip (tempimage1,out1,0);
    
    glDrawPixels( out1.size().width, out1.size().height, GL_BGR, GL_UNSIGNED_BYTE, out1.ptr() );    
    //////////////////////////////////////////////////////////////////////////////////
    // Here, set up new parameters to render a scene viewed from the camera.
    
    //set viewport
    glViewport(0, 0, Img_width, Img_height);
    
    //set projection matrix using intrinsic camera params
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    //gluPerspective is arbitrarily set, you will have to determine these values based
    //on the intrinsic camera parameters
    
    gluPerspective(30, Img_width*1.0/Img_height, 1, 30); 
    
    //you will have to set modelview matrix using extrinsic camera params
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0, 0, 5, 0, 0, 0, 0, 1, 0);  
    
    
    /////////////////////////////////////////////////////////////////////////////////
    // Drawing routine
    
    // now that the camera params have been set, draw your 3D shapes
    // first, save the current matrix
    
    glPushMatrix();
    
    //move to the position where you want the 3D object to go
    glTranslatef(0, 0, 0); 
    
    //this is an arbitrary position for demonstration
    //you will need to adjust your transformations to match the positions where
    //you want to draw your objects(i.e. chessboard center, chessboard corners)
    glColor3f(1.0,1.0,1.0);
    DrawCone ();
    glPopMatrix();
    
    
    // show the rendering on the screen
    glutSwapBuffers();
    
    // post the next redisplay
    glutPostRedisplay();
}
void Jlinkage_app()
{

    IplImage* IGray;
    IplImage* Edge_map;
    CvPoint start_pt, end_pt;
    
    clock_t start, end;
    //double elapsed;
    start = clock();

    vector<vector<CvPoint> > Line_Segmentation;          
    IGray  = cvCreateImage(cvGetSize(frame), IPL_DEPTH_8U, 1);
    Edge_map  = cvCreateImage(cvGetSize(frame), IPL_DEPTH_8U, 1);
    cvCvtColor(frame,  IGray, CV_RGB2GRAY); 
    Line_Segmentation= edge_detection_test(IGray,Edge_map);

    //end = clock();
    //elapsed = ((double) (end - start)) / CLOCKS_PER_SEC;
    
    std::cout<<"time edge  and line segmentation: "<<elapsed <<endl;
    
    //clock_t start, end;
    //double elapsed;
    start = clock();
    
    fit(Line_Segmentation, Img_width,Img_height);
    Edg.~Edge2();
    
    Jlinkage();   
    
    end = clock();
    elapsed = ((double) (end - start)) / CLOCKS_PER_SEC;
    std::cout<<"time j-linkage: "<<elapsed;
    
    IplImage* Img_plot;
    Img_plot= cvCloneImage(frame);
    Plot_frame_lines(Img_plot, Line_Segmentation);
    frame= cvCloneImage(Img_plot);
    erase_2Dvector();
    
    vector<vector<bool> >().swap(errorlst); 

    cvReleaseImage(&Img_plot);
    cvReleaseImage(&IGray);
    cvReleaseImage(&Edge_map);
}

void Plot_frame_lines(IplImage* Img_plot, vector< vector<CvPoint> >& Line_Segmentation)
{
        int size_cluster=(int) New_Cluster.size();
       
        for (int i=0;i<size_cluster;i++)
        {
            int size= (int)New_Cluster[i].size();
            
           for (int j=0;j<size;j++)
            {
            
                int edge_index =  New_Cluster[i][j];
      
                int edgesize= (int) Line_Segmentation[edge_index].size();
            
            //int index1 =1; 
            CvPoint end_pt;
            end_pt.x = Line_Segmentation[edge_index][edgesize-1].x;
            end_pt.y = Line_Segmentation[edge_index][edgesize-1].y;
            CvPoint start_pt;
            start_pt.x = Line_Segmentation[edge_index][0].x;
            start_pt.y = Line_Segmentation[edge_index][0].y;
    
              if(i==0)
              cvLine(Img_plot,start_pt,end_pt,CV_RGB(255,0,0),2.0,CV_AA); 
              if(i==1)
              cvLine(Img_plot,start_pt,end_pt,CV_RGB(0,255,0),2.0,CV_AA);   
              if(i==2)   
              cvLine(Img_plot,start_pt,end_pt,CV_RGB(0,0,255),2.0,CV_AA); 
              }
            }
//    int size_cluster=(int) cluster_result.size();
//   
//    for (int i=0;i<size_cluster;i++)
//    {
//        int size= (int)cluster_result[i].size();
//        
//        for (int j=0;j<size;j++)
//        {
//        
//            int edge_index =  cluster_result[i][j];
//  
//            int edgesize= (int) Line_Segmentation[edge_index].size();
//        
//        //int index1 =1; 
//        CvPoint end_pt;
//        end_pt.x = Line_Segmentation[edge_index][edgesize-1].x;
//        end_pt.y = Line_Segmentation[edge_index][edgesize-1].y;
//        CvPoint start_pt;
//        start_pt.x = Line_Segmentation[edge_index][0].x;
//        start_pt.y = Line_Segmentation[edge_index][0].y;
//
//          if(i==0)
//          cvLine(Img_plot,start_pt,end_pt,CV_RGB(255,0,0),2.0,CV_AA); 
//          if(i==1)
//          cvLine(Img_plot,start_pt,end_pt,CV_RGB(0,255,0),2.0,CV_AA);   
//          if(i==2)   
//          cvLine(Img_plot,start_pt,end_pt,CV_RGB(0,0,255),2.0,CV_AA); 
//          }
//        }
}

void DrawCone ()
{
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   
    glBindTexture(GL_TEXTURE_2D, myTexture);
    glClearColor(0.0f,0.0f,0.0f,0.0f);
    glColor3f(1.0,1.0,1.0);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 0.0); glVertex3f(-0.1,-0.1, 0.0);
    glTexCoord2f(0.0, 1.0); glVertex3f(-0.1, 0.1, 0.0);
    glTexCoord2f(1.0, 1.0); glVertex3f(0.1, 0.1, 0.0);
    glTexCoord2f(1.0, 0.0); glVertex3f(0.1, -0.1, 0.0);
    
    glTexCoord2f(1.0, 1.0); glVertex3f(0.1, 0.1, 0.0);
    glTexCoord2f(0.0, 1.0); glVertex3f(0.1, 0.1, -0.141);
    glTexCoord2f(0.0, 0.0); glVertex3f(0.1, -0.1, -0.141);
    glTexCoord2f(1.0, 0.0); glVertex3f(0.1, -0.1, 0.0);

    glTexCoord2f(1.0f, 1.0f);glVertex3f(-0.1, 0.1, -0.0);
    glTexCoord2f(0.0f, 1.0f);glVertex3f(-0.1, 0.1, -0.141);
    glTexCoord2f(0.0f, 0.0f);glVertex3f(-0.1, -0.1, -0.141);
    glTexCoord2f(1.0f, 0.0f);glVertex3f(-0.1, -0.1, -0.0);
//
    glTexCoord2f(0.0f, 0.0f);glVertex3f(-0.1, -0.1, -0.141);
    glTexCoord2f(0.0f, 1.0f);glVertex3f(-0.1, 0.1, -0.141);
    glTexCoord2f(1.0f, 1.0f);glVertex3f(0.1, 0.1, -0.141);
    glTexCoord2f(1.0f, 0.0f);glVertex3f(0.1, -0.1, -0.141);
//    
    glTexCoord2f(1.0f, 1.0f);glVertex3f(0.1, -0.1, -0.0);
    glTexCoord2f(0.0f, 1.0f);glVertex3f(-0.1,-0.1, -0.0);
    glTexCoord2f(0.0f, 0.0f);glVertex3f(-0.1,-0.1, -0.141);
    glTexCoord2f(1.0f, 0.0f);glVertex3f(0.1, -0.1, -0.141);
    
    
    glEnd();
    glFlush();
    glBindTexture(GL_TEXTURE_2D, 0);
    //glDisable(GL_TEXTURE_2D);
}