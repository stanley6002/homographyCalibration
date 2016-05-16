//
//  Operation_type.h
//  Homography _calibration
//
//  Created by chih-hsiang chang on 2/18/14.
//  Copyright 2014 __MyCompanyName__. All rights reserved.
//


typedef enum {Add_opengl_1=1 , frame_based_1 , image_based_1} MyEnum;
class typp_test
{
public:
    
    
    static inline MyEnum read()
    {
        return (_enum);
    };
    static inline void set_Add_opengl()
    {
        _enum=Add_opengl_1;   
    }
    static inline void set_frame_based()
    {
        _enum=frame_based_1;   
    }
    static inline void set_image_based()
    {
        _enum=image_based_1;   
    }

private:
    static MyEnum _enum;
};