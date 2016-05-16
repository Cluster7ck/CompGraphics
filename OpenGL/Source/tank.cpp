//
//  tank.cpp
//  CG_P1
//
//  Created by  on 30.04.16.
//  Copyright © 2016 hsos. All rights reserved.
//

#include "../Header/tank.h"

Tank::Tank() {
} 

Tank::~Tank() {
}

bool Tank::load(const char* ChassisModel, const char* CanonModel, const Vector& StartPos) {
	g_Model_top.load(ChassisModel, false);
	g_Model_bottom.load(CanonModel, false);
	this->position = StartPos;

	m_Chassis.translation(StartPos);
	m_Canon.translation(StartPos);

	return true;
}

void Tank::steer(float ForwardBackward, float LeftRight) {
	this->route = ForwardBackward;
	this->angle = LeftRight;
}

void Tank::aim(unsigned int MouseX, unsigned int MouseY) {
	/* Berechnen der Pixelkoordinaten des Mauszeigers in normalisierte Bildkoordinaten um
    (x in[-­‐1,1] und y in[-­‐1,1]). Dies entspricht der inversen Viewport‐Matrix.*/
		
	// todo
	float xView = 1.0f*MouseX / (float)g_Camera.getWindowWidth()*2.0f - 1.0f;
	float yView = 1.0f*MouseY / (float)g_Camera.getWindowHeight()*2.0f - 1.0f;
	float zView = 0;
	Vector mouseCoord(xView, yView, zView);
	mouseCoord = mouseCoord.normalize();
	/* Normalisierte Bildkoordinaten nutzen, um einen Richtungsvektor im Kameraraum(View‐Coordinates) 
	zu erzeugen, indem die Projektionsmatrix von g_Camera invers auf die neuen Mauszeigerkoordinaten 
	anwenden. Die Z‐Koordinate ist 0.*/
	Matrix projM(g_Camera.getProjectionMatrix());
	mouseCoord = projM.invert().transformVec4x4(mouseCoord);

	/* Richtungsvektor in Kamera­‐Koordinaten in Weltkoordinaten umrechnen. Richtung des Vektors anpassen 
	und nicht dessen Ursprung. Wenn die Richtung des Strahls in Weltkoordinaten überführt wurde, muss 
	nur noch der Ursprung des Strahls bestimmt werden. Der Ursprung des Strahls ist die Kameraposition, 
	diese ist in der Kameramatrix(g_Camera.getViewMatrix()) kodiert.*/
	Matrix viewM(g_Camera.getViewMatrix());
	mouseCoord = viewM.invert().transformVec4x4(mouseCoord);
	Vector pointA(0, 0, 0);
	Vector pointB(0, 0, 0);
	Vector pointC(0, 0, 0);
	g_Camera.getPosition().triangleIntersection(mouseCoord)
	/* Schnittpunkt mit der Ebene Y=0 berechnen. Die Berechnung des Schnittpunkts wie beim Raytracing-­Verfahren:
	Wähle 3 beliebige Punkte welche auf der y-Ebene liegen --> Algorithmus aus triangleIntersection */

	// todo

	/* Um die Haubitze des Panzers mit homogenen Matrix­‐Transformationen auszurichten, wird die Position des
	Schnittpunkts benutzt.
	Info: Eine affine Matrix kann auch als Koordinatensystem verstanden werden.*/
	
	// todo
}

void Tank::update(float DeltaTime) {
	if (angle != 0)
		angle = angle*DeltaTime*50;

	Matrix mr1, mt;
	mr1.rotationY(angle);

	mt.translation(route*DeltaTime, 0, 0);

	Matrix g = mr1 * mt;
	m_Chassis = m_Chassis * g;
	draw();
}

void Tank::draw() {
	glPushMatrix();
	glMultMatrixf(m_Chassis);
	g_Model_top.drawTriangles();
	g_Model_bottom.drawTriangles();
	glPopMatrix();
}
