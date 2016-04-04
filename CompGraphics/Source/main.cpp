//
//  main.cpp
//  CG_P1
//
//  Created by  on 17.03.16.
//  Copyright © 2016 hsos. All rights reserved.
//

#include <iostream>
#include "../Header/test2.h"
#include "../Header/test1.h"
#include "../Header/test3.h"
#include "../Header/color.hpp"
#include "../Header/CGUtilities.h"
#include "../Header/rgbimage.hpp"
#include "../Header/simpleraytracer.hpp"

int main(int argc, const char * argv[]) {
	Scene ModelScene(20);
	RGBImage Image(640, 480);
	SimpleRayTracer Raytracer(1);
	Raytracer.traceScene(ModelScene, Image);
	Image.saveToDisk("../raytracing_image.bmp");
    return 0;
}
