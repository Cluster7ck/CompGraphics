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
	this->aimTarget = Vector(0, 0, 0);
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
	float xView = 1.0f*MouseX / (float)g_Camera.getWindowWidth()*2.0f - 1.0f;
	float yView = -(MouseY / (float)g_Camera.getWindowHeight()*2.0f - 1.0f);
	float zView = 0;
	Vector mouseCoord(xView, yView, zView);
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
	Matrix invViewM = viewM.invert();
	mouseCoord = invViewM.transformVec3x3(mouseCoord);

	/* Schnittpunkt mit der Ebene Y=0 berechnen. Die Berechnung des Schnittpunkts wie beim Raytracing-­Verfahren:
	Wähle 3 beliebige Punkte welche auf der y-Ebene liegen --> Algorithmus aus triangleIntersection */
	//Plane intersection
	float s = -invViewM.translation().Y / mouseCoord.Y;
	aimTarget = invViewM.translation() + mouseCoord * s;
}

void Tank::update(float DeltaTime) {
	if (angle != 0)
		angle = angle*DeltaTime*5;

	Matrix mr1, mt,mt2,mAim,mr2;
	mr1.rotationY(angle);

	mt.translation(route*DeltaTime, 0, 0);
	Matrix g =  mt * mr1;
	m_Chassis = m_Chassis * g;

	Vector up(0, 1, 0);
	Vector forward = aimTarget - m_Chassis.translation();
	forward.normalize();
	//da rechtshaendig 
	Vector right = up.cross(forward);

	mAim.identity();
	mAim.right(right);
	mAim.up(up);
	mAim.forward(forward);

	mt2.translation(m_Chassis.translation());
	//90 Deg korrektur
	mr2.rotationY(0.5* M_PI *-1);
	m_Canon = mt2 * (mAim * mr2);

	draw();
}

void Tank::draw() {
	glPushMatrix();
	glMultMatrixf(m_Chassis);
	g_Model_bottom.drawTriangles();
	glPopMatrix();

	glPushMatrix();
	glMultMatrixf(m_Canon);
	g_Model_top.drawTriangles();
	glPopMatrix();


}
