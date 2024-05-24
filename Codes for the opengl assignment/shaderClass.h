#ifndef SHADER_CLASS_H
#define SHADER_CLASS_H

#include<iostream>
#include<fstream>
#include<sstream>
#include<glad/glad.h>
#include<cerrno>
#include<string>

std::string get_file_contents(const char* filename);

class Shader
{
public:
	GLuint ID;
	Shader(const char* vertexFile, const char* fragmentFile);

	void Activate();
	void Delete();
private:
	void compileErrors(unsigned int shader, const char* type);
};

#endif