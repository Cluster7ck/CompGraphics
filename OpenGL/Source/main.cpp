//
//  main.cpp
//  RealtimeRending
//
//  Created by Philipp Lensing on 22.10.14.
//  Copyright (c) 2014 Philipp Lensing. All rights reserved.
//

#define _USE_MATH_DEFINES

#include <iostream>
#include <math.h>
#ifdef WIN32
	#include <GL/glew.h>
	#include <GL/GLUT.h>
	#include <GL/GL.h>
#else
	#include <GLUT/GLUT.h>
	#include <OpenGL/OpenGL.h>
#endif
#include "../Header/Camera.h"
#include "../Header/model.h"
#include "../Header/matrix.h"
#include "../Header/tank.h"
#include "../Header/scene.h"

const unsigned int g_WindowWidth=1024;
const unsigned int g_WindowHeight=768;
const Vector g_LightPos = Vector( 0,15,0);

Camera g_Camera;

Model g_Model;

/* Aufgabe 1 */
float deltaTime = 0;
int elapsedTimeLastFrame = 0;

/* Aufgabe 2 */
Tank* g_Model_2 = new Tank();
int keyStore[4];

/* Aufgabe 3 */
Scene* g_Scene = new Scene();

int g_MouseButton = 0;
int g_MouseState = 0;

void SetupDefaultGLSettings();
void DrawScene();
void MouseCallback(int Button, int State, int x, int y);
void MouseMoveCallback(int x, int y);
void KeyboardCallback( unsigned char key, int x, int y);
void MousePassiveMoveCallback( int x, int y);
void SpecialKeyboardCallback( int key, int x, int y);
void SpecialKeyboardUpCallback( int key, int x, int y); 
void TransformAndDrawTank(float dtime);

int main(int argc, char * argv[]) {
    // initialize the glut system and create a window
    glutInitWindowSize(g_WindowWidth, g_WindowHeight);
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
    glutCreateWindow("CG Praktikum");
    
    SetupDefaultGLSettings();
    
    glutDisplayFunc(DrawScene);
    glutMouseFunc(MouseCallback);
    glutKeyboardFunc(KeyboardCallback);
    glutMotionFunc(MouseMoveCallback);
    glutPassiveMotionFunc(MousePassiveMoveCallback);
    glutSpecialFunc(SpecialKeyboardCallback);
    glutSpecialUpFunc(SpecialKeyboardUpCallback);

	/* Aufgabe 1 */
	//g_Model.load("OBJmodels/modelle/tank.obj", false);

	/* Aufgabe 2 */
	Vector* position = new Vector(0, 0, 0);
	g_Model_2->load("OBJmodels/p4_modelle/tank_top.obj", "OBJmodels/p4_modelle/tank_bottom.obj", *position);

	/* Aufgabe 3 */
	g_Scene->addSceneFile("OBJmodels/p4_modelle/scene.osh");
    
    glutMainLoop();
}

void SetupDefaultGLSettings() {
    glClearColor(0.5f, 0.5f, 0.5f, 255);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(65, (double)g_WindowWidth/(double)g_WindowHeight, 0.045f, 1000.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
 
    // Setup Light Color
    GLfloat ambientLight[] = { 0.5f, 0.5f, 0.5f, 0.0f };
    GLfloat diffuseLight[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat specularLight[] = { 1.0f, 1.0f, 1.0f, 0.0f };
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);

    // Setup default material
    float diff[4] = {1,1,1,1};
    float amb[4]  = {0.2f,0.2f,0.2f,1};
    float spec[4] = {0.5f,0.5f,0.5f,1};
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diff);
    glMaterialfv(GL_FRONT, GL_SPECULAR, spec);
    glMateriali(GL_FRONT, GL_SHININESS, 30);
    glMaterialfv(GL_FRONT, GL_AMBIENT, amb);
    
    glShadeModel(GL_SMOOTH);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
}

void DrawGroundGrid() {
    const float GridSize=10.0f;
    const unsigned int GridSegments=20;
    const float GridStep=GridSize/(float)GridSegments;
    const float GridOrigin=-GridSize*0.5f;
    
    glDisable( GL_LIGHTING);
    glBegin(GL_LINES);
        glColor3f(1.0f, 1.0f, 1.0f);
        for( unsigned int i=0; i<GridSegments+1; i++) {
            float itpos=GridOrigin + GridStep*(float)i;
            glVertex3f(itpos, 0, GridOrigin);
            glVertex3f(itpos, 0, GridOrigin+GridSize);
        
            glVertex3f(GridOrigin, 0, itpos);
            glVertex3f(GridOrigin+GridSize, 0, itpos);
        }
    glEnd();
    glEnable( GL_LIGHTING);
}

void MouseCallback(int Button, int State, int x, int y) {
    g_MouseButton = Button;
    g_MouseState = State;
    g_Camera.mouseInput(x,y,Button,State);

}

void MouseMoveCallback( int x, int y) {
    g_Camera.mouseInput(x,y,g_MouseButton,g_MouseState);
}

void MousePassiveMoveCallback( int x, int y) {
    // function is called if the mouse moves without pressing any button.
	g_Model_2->aim(x, y);
}

void KeyboardCallback( unsigned char key, int x, int y) {
    // function is called if a regular keyboard button is pressed
}

void SpecialKeyboardCallback( int key, int x, int y) {
    // function is called if a special keyboard button is pressed (e. g. Up-arrow-Key)
	switch (key) {
	case 100:
		keyStore[0] = -1;
		break;
	case 102:
		keyStore[1] = 1;
		break;
	case 101:
		keyStore[2] = 1;
		break;
	case 103:
		keyStore[3] = -1;
		break;
	default:
		x = y = 0;
		break;
	}
	g_Model_2->steer(keyStore[2] + keyStore[3], keyStore[0] + keyStore[1]);
}

void SpecialKeyboardUpCallback( int key, int x, int y) {
	// function is called if a special keyboard button is pressed (e. g. Up-arrow-Key)
	switch (key) {
	case 100:
		keyStore[0] = 0;
		break;
	case 102:
		keyStore[1] = 0;
		break;
	case 101:
		keyStore[2] = 0;
		break;
	case 103:
		keyStore[3] = 0;
		break;
	default:
		x = y = 0;
		break;
	}
	g_Model_2->steer(keyStore[2] + keyStore[3], keyStore[0] + keyStore[1]);
}

/* Aufgabe 1 */
void TransformAndDrawTank(float dtime) {
	static float Angle = 0;
	// M_PI/180 = degree to rad factor
	Angle += 1 * dtime;

	Matrix mr1, mt, mr2;
	mr1.rotationY(Angle);
	mt.translation(3, 0, 0);
	mr2.rotationY(M_PI_2);

	Matrix g = mr1 * mt * mr2;
	glPushMatrix();
	glMultMatrixf(g);
	g_Model.drawTriangles();
	glPopMatrix();
}

void DrawScene() {
	/* Aufgabe 1 */
	deltaTime = (glutGet(GLUT_ELAPSED_TIME) - elapsedTimeLastFrame) / 1000.0;
	elapsedTimeLastFrame = glutGet(GLUT_ELAPSED_TIME);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    GLfloat lpos[4];
    lpos[0]=g_LightPos.X; lpos[1]=g_LightPos.Y; lpos[2]=g_LightPos.Z; lpos[3]=1;
    glLightfv(GL_LIGHT0, GL_POSITION, lpos);
    
    g_Camera.apply();

	/* Aufgabe 1 */
    //TransformAndDrawTank(deltaTime);
    //DrawGroundGrid();
    
	/* Aufgabe 2 */
	int key = 0;
	int x = 0;
	int y = 0;
	g_Model_2->update(deltaTime);
	/* Aufgabe 3 */
	g_Scene->draw();

    glutSwapBuffers();
    glutPostRedisplay();
}