#include "Shader.h"

#include <fstream>
#include <sstream>
#include <iostream>

#include <engine/core.h>

#include <glad/glad.h>


Shader::Shader(const char* vsPath, const char* fsPath)
	: m_VsPath(vsPath), m_FsPath(fsPath)
{
	LoadFiles();

	// create shaders
	m_RendererID = glCreateProgram();

	unsigned int vs = CompileShader(GL_VERTEX_SHADER, m_VertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, m_FragmentShader);

	glAttachShader(m_RendererID, vs);
	glAttachShader(m_RendererID, fs);

	glLinkProgram(m_RendererID);
	glValidateProgram(m_RendererID);

	//int nShaders;
	//glGetProgramiv(m_RendererID, GL_ATTACHED_SHADERS, &nShaders);
	//std::cout << nShaders << "\n";

	int result;
	glGetProgramiv(m_RendererID, GL_LINK_STATUS, &result);
	if (!result)
	{
		int length;
		glGetProgramiv(m_RendererID, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)alloca(length * sizeof(char));
		glGetProgramInfoLog(m_RendererID, length, &length, message);
		std::cout << message << "\n";
		AB_ASSERT(result, message);
	}

	glDeleteShader(vs);
	glDeleteShader(fs);
}

Shader::~Shader()
{
}

unsigned int Shader::CompileShader(unsigned int type, std::string& source)
{
	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(id, 1, &src, NULL);
	glCompileShader(id);

	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)alloca(length * sizeof(char));
		glGetShaderInfoLog(id, length, &length, message);
		glDeleteShader(id);
		AB_ASSERT(result, "Failed to compile {0} shader \n {1}", (type == GL_VERTEX_SHADER) ? "vertex" : "fragment", message);
	}

	return id;
}


void Shader::Bind() const
{
	glUseProgram(m_RendererID);
}

void Shader::Unbind() const
{
	glUseProgram(0);
}

void Shader::SetUniform1i(const std::string& name, const int& value)
{
	glUniform1i(GetUniformLocation(name), value);
}

void Shader::SetUniform1f(const std::string& name, const float& value)
{
	glUniform1f(GetUniformLocation(name), value);
}

void Shader::SetUniform4f(const std::string& name, const glm::vec4 &vec4)
{
	glUniform4f(GetUniformLocation(name), vec4.x, vec4.y, vec4.z, vec4.w);
}

void Shader::SetUniform4mat(const std::string& name, const glm::mat4 &mat)
{
	glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &mat[0][0]);
}

unsigned int Shader::GetUniformLocation(const std::string& name)
{
	if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
		return m_UniformLocationCache[name];

	int location = glGetUniformLocation(m_RendererID, name.c_str());

	if (location == -1)
		AB_WARN("Warning: uniform {0} doesn't exists", name);

	m_UniformLocationCache[name] = location;
	return location;
}


void Shader::LoadFiles()
{
	std::ifstream vs(m_VsPath);
	std::ifstream fs(m_FsPath);

	// load vertex shader
	AB_ASSERT(vs.is_open(), "could not open vertex shader file at path {0}", m_VsPath);

	std::string line;
	std::stringstream ss;

	while (getline(vs, line))
		ss << line << '\n';

	m_VertexShader = ss.str();
	vs.close();

	// load fragment shader
	AB_ASSERT(fs.is_open(), "could not open fragment shader file at path {0}", m_FsPath);

	std::string line2;
	std::stringstream ss2;

	while (getline(fs, line2))
		ss2 << line2 << '\n';
	
	m_FragmentShader = ss2.str();
	fs.close();
}