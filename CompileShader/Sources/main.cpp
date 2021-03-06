//
//  main.cpp
//  RealtimeRending
//
//  Created by Philipp Lensing on 22.10.14.
//  Copyright (c) 2014 Philipp Lensing. All rights reserved.
//

#ifdef WIN32
	#include <windows.h>
	#include <GL/glew.h>
	#include <GL/GLUT.h>
	#include <GL/GL.h>
#else
	#include <OpenGL/OpenGL.h>
	#include <GLUT/GLUT.h>
#endif
#include <iostream>
#include <Windows.h>
#include <WinBase.h>

#include "../Header/shaderprogram.h"
#include "../Header/Camera.h"
#include "../Header/model.h"
#include "../Header/texture.h"

// window x and y size
const unsigned int g_WindowWidth = 1024;
const unsigned int g_WindowHeight = 768;

// Aufgabe 3

// Model that should be loaded
//const char* g_ModelToLoad = "OBJmodels/conference/conference.obj";
const char* g_ModelToLoad = "OBJmodels/sibenik/sibenik.obj"; 
//const char* g_ModelToLoad = "OBJmodels/sibenik/sponza.obj";
//const char* g_ModelToLoad = "OBJmodels/sponza/sponza.obj";
//const char* g_ModelToLoad = "OBJmodels/cube.obj";
//const char* g_ModelToLoad = "OBJmodels/figure.obj";

// light position (point light)
const Vector g_LightPos = Vector(0, 16, 0);

Camera g_Camera;
Model g_Model;
int g_MouseButton = 0;
int g_MouseState = 0;

void SetupDefaultGLSettings();
void DrawScene();
void MouseCallback(int Button, int State, int x, int y);
void MouseMoveCallback(int x, int y);
void KeyboardCallback(unsigned char key, int x, int y);

enum RenderMode {
	RENDERMODE_LINES,
	RENDERMODE_TRIANGLES,
	LAST_RENDERMODE
};

RenderMode g_RenderMode = RENDERMODE_LINES;

int main(int argc, char * argv[]) {
	// initialize the glut system and create a window
	glutInitWindowSize(g_WindowWidth, g_WindowHeight);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
	glutCreateWindow("CG Praktikum");
#ifdef WIN32
	glewInit();
#endif

	SetupDefaultGLSettings();

	glutDisplayFunc(DrawScene);
	glutMouseFunc(MouseCallback);
	glutKeyboardFunc(KeyboardCallback);
	glutMotionFunc(MouseMoveCallback);

	//g_Model.load(g_ModelToLoad, true);
	g_Model.loadWithShader(g_ModelToLoad, "Shader/vertexshader.glsl", "Shader/blinn_phong_fragmentshader.glsl", true);
	//g_Model.loadWithShader(g_ModelToLoad, "Shader/vertexshader.glsl", "Shader/toon_fragmentshader.glsl", true);
	
	glutMainLoop();
}

void SetupDefaultGLSettings() {
	glClearColor(0, 0, 0, 255);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(65, (double)g_WindowWidth / (double)g_WindowHeight, 0.045f, 1000.0f);

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
	float diff[4] = { 1,1,1,1 };
	float amb[4] = { 0.2f,0.2f,0.2f,1 };
	float spec[4] = { 0.5f,0.5f,0.5f,1 };
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diff);
	glMaterialfv(GL_FRONT, GL_SPECULAR, spec);
	glMateriali(GL_FRONT, GL_SHININESS, 30);
	glMaterialfv(GL_FRONT, GL_AMBIENT, amb);

	glShadeModel(GL_SMOOTH);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
}

void DrawGroundGrid() {
	const float GridSize = 10.0f;
	const unsigned int GridSegments = 20;
	const float GridStep = GridSize / (float)GridSegments;
	const float GridOrigin = -GridSize*0.5f;

	glDisable(GL_LIGHTING);
	glBegin(GL_LINES);
	glColor3f(1.0f, 1.0f, 1.0f);
	for (unsigned int i = 0; i<GridSegments + 1; i++) {
		float itpos = GridOrigin + GridStep*(float)i;
		glVertex3f(itpos, 0, GridOrigin);
		glVertex3f(itpos, 0, GridOrigin + GridSize);

		glVertex3f(GridOrigin, 0, itpos);
		glVertex3f(GridOrigin + GridSize, 0, itpos);
	}
	glEnd();
	glEnable(GL_LIGHTING);
}

void MouseCallback(int Button, int State, int x, int y) {
	g_MouseButton = Button;
	g_MouseState = State;
	g_Camera.mouseInput(x, y, Button, State);
}

void MouseMoveCallback(int x, int y) {
	g_Camera.mouseInput(x, y, g_MouseButton, g_MouseState);
}

void KeyboardCallback(unsigned char key, int x, int y) {
	if (key == 'l')
		g_RenderMode = RENDERMODE_LINES;
	else if (key == 't')
		g_RenderMode = RENDERMODE_TRIANGLES;
}


void DrawScene() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	g_Camera.apply();

	//DrawGroundGrid();

	GLfloat lpos[4];
	lpos[0] = g_LightPos.X; lpos[1] = g_LightPos.Y; lpos[2] = g_LightPos.Z; lpos[3] = 1;
	glLightfv(GL_LIGHT0, GL_POSITION, lpos);

	if (g_RenderMode == RENDERMODE_LINES) {
		glDisable(GL_LIGHTING);
		g_Model.drawLines();
		glEnable(GL_LIGHTING);
	}
	else if (g_RenderMode == RENDERMODE_TRIANGLES)
		g_Model.drawTriangles();

	glutSwapBuffers();
	glutPostRedisplay();
}

// Aufgabe 2
/*
void SetupDefaultGLSettings();

int main(int argc, char * argv[]) {

	// initialize the glut system and create a window
	
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowSize(g_WindowWidth, g_WindowHeight);
	glutCreateWindow("CG Praktikum");
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (GLEW_OK != err) {
		std::cerr << "Error: " << glewGetErrorString(err) << std::endl;
		exit(0);
	}

	if (argc < 3) {
		std::cout << "Use -f and/or -v to declare shaders to be compiled."<< std::endl;
		//keep open
		std::cin.get();
		exit(0);
	}

	char* fragShader;
	char* vertexShader;

	for (int i = 1; i < argc; i++) {
		if (i + 1 != argc) {
			if (strcmp(argv[i], "-f") == 0) {
				fragShader = argv[i + 1];
				std::cout << fragShader << std::endl;
				i++;
			}
			else if (strcmp(argv[i],"-v") == 0) {
				vertexShader = argv[i + 1];
				std::cout << vertexShader << std::endl;
				i++;
			}
			else {
				std::cout << argv[i] << std::endl;
				std::cout << "Invalid argument." << std::endl;
				Sleep(2000);
				exit(0);
			}
		}
	}

	ShaderProgram s_Prog;
	//s_Prog.load(vertexShader, fragShader);
	s_Prog.loadVertexShader(vertexShader);
	std::string compileErrors;
	s_Prog.compile(&compileErrors);

	std::cout << compileErrors << std::endl;
	std::cin.get();
}
*/