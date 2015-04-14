#pragma once
#include "GL/glew.h"
#include <GL/freeglut.h>
#include <map>
#include <string>

class GLSLShader
{
public:
	GLSLShader();
	~GLSLShader();

	void LoadFromString(GLenum whichShader, const std::string& source);
	void LoadFromFile(GLenum whichShader, const std::string& fileName);
	void CreateAndLinkProgram();
	void DeleteShaderProgram();
	void Use();
	void UnUse();
	void AddAttribute(const std::string& attribute);
	void AddUniform(const std::string& uniform);
	GLuint operator[](const std::string& attribute);
	GLuint operator()(const std::string& uniform);
private:
	enum ShaderType 
	{
		VERTEX_SHADER, FRAGMENT_SHADER, GEOMETRY_SHADER
	};

	GLuint _program;
	int _totalShaders;
	GLuint _shaders[3];
	std::map<std::string, GLuint> _attributeList;
	std::map<std::string, GLuint> _uniformLocationList;
};

