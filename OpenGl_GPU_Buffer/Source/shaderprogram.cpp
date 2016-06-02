//
//  shaderprogram.cpp
//  CG_5
//
//  Created by  on 02.06.16.
//  Copyright © 2016 hsos. All rights reserved.
//

#include "../Header/shaderprogram.h"

unsigned long getFileLength(std::ifstream& file);

ShaderProgram::ShaderProgram(){}
ShaderProgram::~ShaderProgram(){}

bool ShaderProgram::load(const char* VertexShader, const char* FragmentShader){
	loadVertexShader(VertexShader);
	loadFragmentShader(FragmentShader);

    return true;
}

bool ShaderProgram::loadVertexShader(const char* VertexShader){
    GLubyte* ShaderSource;
    
    std::ifstream file;
    file.open(VertexShader, file.in); // opens as ASCII!
    unsigned long len = getFileLength(file);
    if(!file) return false;
    
    len = getFileLength(file);
    
    if (len==0) return false;   // Error: Empty File
    
    ShaderSource = (GLubyte*) new char[len+1];
    if (ShaderSource == 0) return false;   // can't reserve memory
    
    // len isn't always strlen cause some characters are stripped in ascii read...
    // it is important to 0-terminate the real length later, len is just max possible value...
    ShaderSource[len] = 0;
    
    unsigned int i=0;
    while (file.good())
    {
        ShaderSource[i] = file.get();       // get character from file.
        if (!file.eof())
            i++;
    }
    
    ShaderSource[i] = 0;  // 0-terminate it at the correct position
    
    file.close();
    
    m_VertexShader = glCreateShader(GL_VERTEX_SHADER);

    GLint length = (GLint) strlen((const char*)ShaderSource);
    glShaderSource(m_VertexShader, 1, (const GLcharARB **)&ShaderSource, &length);

    return true; // No Error

}

bool ShaderProgram::loadFragmentShader(const char* FragmentShader) {
	GLubyte* ShaderSource;

	std::ifstream file;
	file.open(FragmentShader, file.in);
	unsigned long len = getFileLength(file);
	if (!file) return false;

	len = getFileLength(file);

	if (len == 0) return false;

	ShaderSource = (GLubyte*) new char[len + 1];
	if (ShaderSource == 0) return false;

	ShaderSource[len] = 0;

	unsigned int i = 0;
	while (file.good())
	{
		ShaderSource[i] = file.get();
		if (!file.eof())
			i++;
	}

	ShaderSource[i] = 0;

	file.close();

	m_FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	GLint length = (GLint)strlen((const char*)ShaderSource);
	glShaderSource(m_FragmentShader, 1, (const GLcharARB **)&ShaderSource, &length);

	return true;

}


unsigned long getFileLength(std::ifstream& file){
    if(!file.good()) return 0;
    
    unsigned long pos=file.tellg();
    file.seekg(0,file.end);
    unsigned long len = file.tellg();
    file.seekg(file.beg);
    
    return len;
}
