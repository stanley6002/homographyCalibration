//
//  Utility.h
//  Homography _calibration
//
//  Created by chih-hsiang chang on 1/28/14.
//  Copyright 2014 __MyCompanyName__. All rights reserved.
//



//#ifndef JLINKAGE_H
//#define JLINKAGE_H

#include <opencv/cv.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv/cxcore.h>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/features2d/features2d.hpp"


#include "Dtype .h"
#include "fit.h"
#include "Jlinkage.h"
#include "VP_linear.h"


#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
extern  CvCapture *camCapture;
extern CvCapture* capture;
extern IplImage*  frame ;
extern int Img_width;
extern int Img_height;
extern bool update_value;
extern float _matrix[9];
extern GLuint  myTexture;

void Plot_frame_lines(IplImage* Img_plot, vector< vector<CvPoint> >& Line_Segmentation);
void display(); 
void display1(); 
void mouse( int button, int state, int x, int y );
void keyboard( unsigned char key, int x, int y );
void idle();
void idle1();
void drawAxes(float length);
void reshape( int w, int h );
//IplImage* skipNFrames(CvCapture* capture, int n);
IplImage* scaleFrame(CvCapture* capture, int n);
void Jlinkage_app();
void DrawCone ();


//#endif /* JLINKAGE_H*/