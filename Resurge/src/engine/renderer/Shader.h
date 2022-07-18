#pragma once

#include <string>
#include <glm/glm.hpp>
#include <unordered_map>
#include <engine/Log.h>

class Shader
{
public:

	Shader(const char* vsPath, const char* fsPath);
	~Shader();

	void Bind() const;
	void Unbind() const;

	// Uniforms Setters
	void SetUniform1i(const std::string& name, const int& value);
	void SetUniform1f(const std::string& name, const float& value);
	void SetUniform4f(const std::string& name, const glm::vec4& vec4);
	void SetUniform4mat(const std::string& name, const glm::mat4& mat);

private:
	unsigned int m_RendererID;

	std::string m_VsPath;
	std::string m_FsPath;

	std::unordered_map<std::string, int> m_UniformLocationCache;

	unsigned int GetUniformLocation(const std::string& name);
	unsigned int CompileShader(unsigned int type, std::string& source);

	std::string m_VertexShader;
	std::string m_FragmentShader;
	
	void LoadFiles();

};