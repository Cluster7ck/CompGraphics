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

int main(int argc, const char * argv[]) {
    // insert code here...
    Test3::rgbimage("..\\");
    Test1::vector();
    Test2::color();
	int a;
	std::cin >> a;
    return 0;
}
