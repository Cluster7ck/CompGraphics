//
//  tank.h
//  CG_P1
//
//  Created by  on 30.04.16.
//  Copyright © 2016 hsos. All rights reserved.
//

#ifndef tank_h
#define tank_h

#define _USE_MATH_DEFINES

#include "../Header/vector.h"
#include "../Header/Model.h"
#include "../Header/Matrix.h"
#include "../Header/Camera.h"

extern Camera g_Camera;

class Tank {
public:
	Model g_Model_top;
	Model g_Model_bottom;
	Vector position;
	float route = 0.0;
	float angle = 0.0;
	Matrix m_Chassis;
	Matrix m_Canon;

	Tank();
	~Tank();
	bool load(const char* ChassisModel, const char* CanonModel, const Vector& StartPos);
	void steer(float ForwardBackward, float LeftRight);
	void aim(unsigned int MouseX, unsigned int MouseY);
	void update(float DeltaTime);
	void draw();
};

#endif /* tank_h */
