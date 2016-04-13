//
//  camera.hpp
//  CG_P1
//
//  Created by  on 31.03.16.
//  Copyright © 2016 hsos. All rights reserved.
//

#ifndef camera_hpp
#define camera_hpp

#include <stdio.h>
#include "vector.hpp"

class Camera{
public:
    Camera(float zvalue, float planedist, float width, float height, float widthInPixel, float heightInPixel);
    Vector generateRay(unsigned int x, unsigned int y) const;
    Vector Position() const;
protected:
    float planedist;
    //Raumeinheiten
    float width;
    float height;
    float widthConversion;
    float heightConversion;
    float zvalue;
    float widthInPixel;
    float heightInPixel;
};

#endif /* camera_hpp */
