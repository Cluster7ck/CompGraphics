//
//  camera.cpp
//  CG_P1
//
//  Created by  on 31.03.16.
//  Copyright © 2016 hsos. All rights reserved.
//

#include "../Header/camera.hpp"

Camera::Camera(float zvalue, float planedist, float width, float height, float widthInPixel, float heightInPixel){
    this->zvalue = zvalue;
    this->planedist = planedist;
    this->width = width;
    this->height = height;
    this->widthInPixel = widthInPixel;
    this->heightInPixel = heightInPixel;
    this->heightConversion = height/heightInPixel;
    this->widthConversion = width/widthInPixel;
}

Vector Camera::generateRay(unsigned int x, unsigned int y) const{
    float xPlane = (x - widthInPixel/2) * widthConversion;
    float yPlane = (y - heightInPixel/2) * heightConversion;
    
    Vector xVec(1,0,0);
    Vector yVec(0,1,0);
    Vector zVec(0,0,1);
    
    Vector p = zVec*(zvalue+planedist);
    
	return (p + xVec * xPlane + yVec * yPlane).normalize();

}

Vector Camera::Position() const{
    return *(new Vector(0,0,zvalue));
}
