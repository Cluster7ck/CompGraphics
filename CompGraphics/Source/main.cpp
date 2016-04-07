//
//  main.cpp
//  CG_P1
//
//  Created by  on 17.03.16.
//  Copyright © 2016 hsos. All rights reserved.
//

#include <iostream>
#include <ctime>

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

	clock_t start_s = clock();
	Raytracer.traceScene(ModelScene, Image);
	clock_t end_s = clock();
	std::cout << "triangle count: " << ModelScene.getTriangleCount() << std::endl;
	std::cout << "trace time: " << ((end_s-start_s) / CLOCKS_PER_SEC) << std::endl;
	Image.saveToDisk("../raytracing_image.bmp");
	int a = 0;
	std::cin >> a ;
    return 0;
}
