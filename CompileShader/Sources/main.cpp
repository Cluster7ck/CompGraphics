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

const unsigned int g_WindowWidth = 1024;
const unsigned int g_WindowHeight = 768;
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