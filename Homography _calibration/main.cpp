//
//  main.cpp
//  Homography _calibration
//
//  Created by chih-hsiang chang on 8/6/13.
//  Copyright 2013 __MyCompanyName__. All rights reserved.
//


#include "Utility.h"
#include "edge_detection.h"
#include "Jlinkage.h"
#include "Operation_type.h"

#include <iostream> 
#include <stdio.h>
#include <stdlib.h>
#include <iomanip>
#include <math.h>
#include "SOIL.h"
#include <ctime>


#define CAMERA_OUTPUT_WINDOW_NAME "camera-output"
#define IMAGE_WIDTH  400
#define IMAGE_HEIGHT 400

int win_id[2];

# define OPENGL_WINDOW 0
# define CLOSE2GL_WINDOW 1
#define Read_from_File


//#define External_Camera_Capture

#define Ues_simulated_frame

MyEnum typp_test::_enum = Add_opengl_1;
GLuint  myTexture;

using namespace std;
void Draw_Result_Imagebased(IplImage *img, int &size_1, vector<vector<CvPoint> > &edgelist);

int main (int argc, char * argv[])
{
  
      typp_test::set_Add_opengl();     // select the operation type here. 

#ifdef External_Camera_Capture
    
    frame=NULL;
    int ret = 0;
    
    if (!(camCapture = cvCaptureFromCAM(CV_CAP_ANY))) 
    {
        cout << "Failed to capture from camera" << endl;
        
        ret = 1;
        
       // goto exitCameraOpenFailed;
    }
    
     cout << "Camera opened successfully" << endl;
      
     IplImage *cameraFrame;    
     
     cameraFrame = cvCreateImage(cvSize(IMAGE_WIDTH,IMAGE_HEIGHT), IPL_DEPTH_8U,1);
     //cvNamedWindow(CAMERA_OUTPUT_WINDOW_NAME, CV_WINDOW_NORMAL);
     //cvNamedWindow("test", CV_WINDOW_NORMAL);
    
     int grabFrameRet;
    
    // while (true) 
    //do 
    //{
        if ((cameraFrame = cvQueryFrame(camCapture))) 
       {
            glutInit(&argc, argv);
            CvPoint start_pt, end_pt;
            
            // capture first frame
            /*
             set up window enviorment
             */ 
            glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE );
            glutInitWindowPosition( 20, 20 );

            //  IplImage* out = cvCreateImage( cvGetSize(cameraFrame), IPL_DEPTH_8U, 3 );
            //  cvCvtColor(cameraFrame ,out, CV_RGB2GRAY);
            //  cvSmooth(cameraFrame, out, CV_GAUSSIAN, 21, 21 );
 //           vector<vector<CvPoint> > Line_Segmentation;
 //CvPoint start_pt, end_pt;
 //           IplImage* IGray;
 //           IplImage* Edge_map;
 //           IGray  = cvCreateImage(cvGetSize(cameraFrame), IPL_DEPTH_8U, 1);
 //           Edge_map  = cvCreateImage(cvGetSize(cameraFrame), IPL_DEPTH_8U, 1);
 //           cvCvtColor(cameraFrame,  IGray, CV_RGB2GRAY); 
 //           Line_Segmentation= edge_detection_test(IGray,Edge_map);
           
            //frame = skipNFrames(camCapture, 10);
            //frame=cameraFrame; 
           frame = cvQueryFrame(camCapture);
           Img_width= frame->width;
           Img_height= frame->height;
            //cvShowImage("test",cameraFrame);
           glutInitWindowSize( Img_width, Img_height );
           win_id[OPENGL_WINDOW]=glutCreateWindow( "OpenGL / OpenCV Example" );
           //glEnable(GL_TEXTURE_2D);
           myTexture = SOIL_load_OGL_texture("/sam.jpg", 0, 1, SOIL_FLAG_POWER_OF_TWO | SOIL_FLAG_INVERT_Y);
           //glBindTexture(GL_TEXTURE_2D, myTexture);
           //glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
           //glDisable(GL_TEXTURE_2D); 
           //glEnable(GL_DEPTH_TEST);

           
            glutDisplayFunc( display );
            glutReshapeFunc( reshape );
            glutMouseFunc( mouse );
            glutKeyboardFunc( keyboard );
            glutIdleFunc( idle1 );        
            glutMainLoop();
            //cvReleaseCapture(&camCapture);
            cvReleaseImage(&cameraFrame);
            cvReleaseImage(&frame);
            
//            for (int i=0;i<(int)Line_Segmentation.size();i++)
//            {
//                
//                start_pt.x =  Line_Segmentation[i][0].x;
//                end_pt.x =    Line_Segmentation[i][int(Line_Segmentation[i].size())-1].x;
//                
//                start_pt.y =  Line_Segmentation[i][0].y;
//                end_pt.y =    Line_Segmentation[i][int(Line_Segmentation[i].size())-1].y;
//                
//                cvLine(frame,start_pt,end_pt,CV_RGB(0,255,0),1,CV_AA);
//            }
              //cvResizeWindow( CAMERA_OUTPUT_WINDOW_NAME, 600, 600 ); 
              //cvResizeWindow( "test", 600,600 ); 
            //  cvShowImage(CAMERA_OUTPUT_WINDOW_NAME,frame);
            //  cvShowImage("test",IGray);
         }
//         if (cvWaitKey(60) != -1)
//         {
//            cout << "Input" << endl;
//            break;
//        }
//     }

//    cout << "Done" << endl;
  
   
//     }
    
    //cvDestroyWindow(CAMERA_OUTPUT_WINDOW_NAME);
    //cvDestroyWindow("test");
    //exitCameraOpenFailed:
//    return 0;
#endif  
#ifdef Ues_simulated_frame   
     capture = cvCaptureFromFile("/test_w_5.mov");
     frame = NULL;

    if (typp_test::read()==2)   // frame_based
    {
         /**
         *    single image capture in this section 
         *    image frame is captured frame based with cvShowImage.
         *    release the memory of frame and capture
         */
        vector<vector<CvPoint> > edgelist;
        IplImage* Edge_map; 
        IplImage* IGray;
        vector<vector<CvPoint> > Line_Segmentation;
        CvPoint start_pt, end_pt;
        do
        {
            frame = skipNFrames(capture, 2);
            cvNamedWindow("frame", CV_WINDOW_NORMAL);
            cvResizeWindow("frame", 400, 400 ); 
            Img_width  = frame->width;
            Img_height = frame->height; 
            IGray  = cvCreateImage(cvGetSize(frame), IPL_DEPTH_8U, 1);
            Edge_map  = cvCreateImage(cvGetSize(frame), IPL_DEPTH_8U, 1);
            cvCvtColor(frame,  IGray, CV_RGB2GRAY); 
            Line_Segmentation= edge_detection_test(IGray,Edge_map);
            
            for (int i=0;i<(int)Line_Segmentation.size();i++)
            {
                
            start_pt.x =  Line_Segmentation[i][0].x;
            end_pt.x =    Line_Segmentation[i][int(Line_Segmentation[i].size())-1].x;
            
            start_pt.y =  Line_Segmentation[i][0].y;
            end_pt.y =    Line_Segmentation[i][int(Line_Segmentation[i].size())-1].y;
            
            cvLine(frame,start_pt,end_pt,CV_RGB(0,255,0),1,CV_AA);
            
            }
            
            //fit(Line_Segmentation, Img_width,Img_height);
            //Jlinkage();
            cvShowImage("frame",frame);
            //cvWaitKey();
        } 
          while( frame != NULL );
          cvReleaseImage(&IGray);
          cvReleaseImage(&Edge_map);
          cvReleaseCapture(&capture);
          cvDestroyWindow("frame");
          cvReleaseImage(&frame);
        
    }
   
     /*
     Initialize GLUT
     Initialize the OpenGL display window
     */

    if (typp_test::read()==1)  // set to Add_opengl
    {
      # define vector_erase 
      //IplImage* IGray;
      //IplImage* Edge_map;
      //vector<vector<CvPoint> > Line_Segmentation;
       glutInit(&argc, argv);
       CvPoint start_pt, end_pt;

      // capture first frame
      /*
        set up window enviorment
      */ 
       glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE );
       glutInitWindowPosition( 20, 20 );
       
       /*
       overwrite the image size for display window 
       */ 
        frame = skipNFrames(capture, 0);
        Img_width= frame->width;
        Img_height= frame->height;
        //cvShowImage("test",cameraFrame);
        glutInitWindowSize( Img_width, Img_height );
        win_id[OPENGL_WINDOW]=glutCreateWindow( "OpenGL / OpenCV Example" );
         myTexture = SOIL_load_OGL_texture("/utd.jpg", 0, 1, SOIL_FLAG_POWER_OF_TWO | SOIL_FLAG_INVERT_Y);
        // set up GUI callback functions
        glutDisplayFunc( display );
        glutReshapeFunc( reshape );
        glutMouseFunc( mouse );
        glutKeyboardFunc( keyboard );
        glutIdleFunc( idle );        
        glutMainLoop();
        cvReleaseCapture(&capture);
        cvReleaseImage(&frame);
        }
    if (typp_test::read()==3)  // set to image_base 
    {
        int ImgWidth=0;
        int ImgHeight=0;
        IplImage *img=cvLoadImage("/b_104.jpg",1);
        ImgWidth =(int) img->width;
        ImgHeight=(int) img->height;
        Image<uint8> imgT(img);
        
        //int DN1;
        //DN1 = imgT(10,0);
        //std::vector<CvPoint> vPt;
        //std::cout<<(DN1)<<endl;
        //std::cout<<ImgWidth<<" "<<ImgHeight<<endl;
        
        IplImage* Edge_map; 
        IplImage* IGray  = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 1);
        Edge_map  = cvCreateImage(cvGetSize(IGray), IPL_DEPTH_8U, 1);
        cvCvtColor(img,  IGray, CV_RGB2GRAY);   
        vector<vector<CvPoint> > edgelist;
        edgelist=edge_detection_test(IGray, Edge_map);
        int start_s=clock();
        fit(edgelist,ImgWidth,ImgHeight);
        Jlinkage(); 
        int stop_s=clock();
        cout << "time: " << (stop_s-start_s)/double(CLOCKS_PER_SEC)*1000 << endl;
       
    }
    #endif
    return 0;   

}
void Draw_Result_Imagebased(IplImage *img, int& size1_ , vector<vector <CvPoint> > &edgelist)
{
    cout<< size1_<<endl;

    for (int i=0;i<size1_;i++)
       
    {
            int size= (int)cluster_result[i].size();
            std::cout<< "cluster "<<i<<" cluster size "<<  size<<endl;
            if (size >1 )
            for (int j=0;j<size;j++)
             { 
               int index = cluster_result[i][j];             
               int edgesize=  edgelist[index].size();
               CvPoint end_pt;
               end_pt.x = edgelist[index][edgesize-1].x;
               end_pt.y = edgelist[index][edgesize-1].y;
               CvPoint start_pt;
               start_pt.x = edgelist[index][0].x;
               start_pt.y = edgelist[index][0].y;
                 cvLine(img,start_pt,end_pt,CV_RGB(0,255,0),1,CV_AA);
             }
    }
}
