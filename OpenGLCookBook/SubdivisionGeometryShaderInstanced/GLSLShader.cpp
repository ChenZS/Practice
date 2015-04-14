#include "GLSLShader.h"
#include <iostream>
#include <fstream>

using namespace std;

GLSLShader::GLSLShader()
: _totalShaders(0)
{
	_shaders[VERTEX_SHADER] = 0;
	_shaders[FRAGMENT_SHADER] = 0;
	_shaders[GEOMETRY_SHADER] = 0;
	_attributeList.clear();
	_uniformLocationList.clear();
}


GLSLShader::~GLSLShader()
{
	_attributeList.clear();
	_uniformLocationList.clear();
}

void GLSLShader::LoadFromString(GLenum type, const std::string& source)
{
	GLuint shader = glCreateShader(type);

	const char * ptmp = source.c_str();
	glShaderSource(shader, 1, &ptmp, NULL);

	//check whether the shader loads fine
	GLint status;
	glCompileShader(shader);
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE) {
		GLint infoLogLength;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);
		char *infoLog = new char[infoLogLength];
		glGetShaderInfoLog(shader, infoLogLength, NULL, infoLog);
		std::cerr << "Compile log: " << infoLog << std::endl;
		delete[] infoLog;
	}
	_shaders[_totalShaders++] = shader;
}

void GLSLShader::LoadFromFile(GLenum type, const string& filename)
{
	ifstream fp;
	fp.open(filename.c_str(), ios_base::in);
	if (fp)
	{
		string line, buffer;
		while (getline(fp, line))
		{
			buffer.append(line);
			buffer.append("\r\n");
		}
		LoadFromString(type, buffer);
	}
	else
	{
		cerr << "Error loading shader: " << filename << endl;
	}
}

void GLSLShader::CreateAndLinkProgram()
{
	_program = glCreateProgram();
	if (_shaders[VERTEX_SHADER] != 0)
	{
		glAttachShader(_program, _shaders[VERTEX_SHADER]);
	}
	if (_shaders[FRAGMENT_SHADER] != 0) {
		glAttachShader(_program, _shaders[FRAGMENT_SHADER]);
	}
	if (_shaders[GEOMETRY_SHADER] != 0) {
		glAttachShader(_program, _shaders[GEOMETRY_SHADER]);
	}

	GLint status;
	glLinkProgram(_program);
	glGetProgramiv(_program, GL_LINK_STATUS, &status);
	if (status == GL_FALSE) {
		GLint infoLogLength;

		glGetProgramiv(_program, GL_INFO_LOG_LENGTH, &infoLogLength);
		char *infoLog = new char[infoLogLength];
		glGetProgramInfoLog(_program, infoLogLength, NULL, infoLog);
		std::cerr << "Link log: " << infoLog << std::endl;
		delete[] infoLog;
	}

	glDeleteShader(_shaders[VERTEX_SHADER]);
	glDeleteShader(_shaders[FRAGMENT_SHADER]);
	glDeleteShader(_shaders[GEOMETRY_SHADER]);
}

void GLSLShader::DeleteShaderProgram()
{
	glDeleteProgram(_program);
}

void GLSLShader::Use()
{
	glUseProgram(_program);
}

void GLSLShader::UnUse()
{
	glUseProgram(0);
}

void GLSLShader::AddAttribute(const std::string& attribute)
{
	_attributeList[attribute] = glGetAttribLocation(_program, attribute.c_str());
}

void GLSLShader::AddUniform(const std::string& uniform)
{
	_uniformLocationList[uniform] = glGetUniformLocation(_program, uniform.c_str());
}

GLuint GLSLShader::operator[](const std::string& attribute)
{
	return _attributeList[attribute];
}

GLuint GLSLShader::operator()(const std::string& uniform)
{
	return _uniformLocationList[uniform];
}