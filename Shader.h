#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>

#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

class Shader
{
public:
	unsigned int m_ID;
public:
	Shader(const char* vertexPath, const char* fragmentPath);

	void use() const;

	template<typename T>
	void setUniform(const std::string& name, const T& value) const;
};

Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
	std::string vertexShaderSource, fragmentShaderSource;
	std::ifstream vertexShaderFile, fragmentShaderFile;
	vertexShaderFile.exceptions(std::istream::badbit | std::istream::failbit);
	fragmentShaderFile.exceptions(std::istream::badbit | std::istream::failbit);
	try
	{
		vertexShaderFile.open(vertexPath);
		fragmentShaderFile.open(fragmentPath);

		std::stringstream vertexShaderStream, fragmentShaderStream;
		vertexShaderStream << vertexShaderFile.rdbuf();
		fragmentShaderStream << fragmentShaderFile.rdbuf();

		vertexShaderFile.close();
		fragmentShaderFile.close();

		vertexShaderSource = vertexShaderStream.str();
		fragmentShaderSource = fragmentShaderStream.str();
	}
	catch (std::ifstream::failure exception)
	{
		std::cout << "ERROR: SHADER::FAILED TO READ SHADER FILE\n";
	}

	const char* vertexShaderCode{ vertexShaderSource.c_str() }, * fragmentShaderCode{ fragmentShaderSource.c_str() };

	unsigned int vertex{ glCreateShader(GL_VERTEX_SHADER) }, fragment{ glCreateShader(GL_FRAGMENT_SHADER) };
	glShaderSource(vertex, 1, &vertexShaderCode, nullptr);
	glCompileShader(vertex);
	int success;
	char infoLog[512];
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertex, 512, nullptr, infoLog);
		std::cout << "ERROR: SHADER::\"VERTEX SHADER\" COMPILE ERROR\n---------------------------\n" << infoLog << '\n';
	}

	glShaderSource(fragment, 1, &fragmentShaderCode, nullptr);
	glCompileShader(fragment);
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragment, 512, nullptr, infoLog);
		std::cout << "ERROR: SHADER::\"FRAGMENT SHADER\" COMPILE ERROR\n---------------------------\n" << infoLog << '\n';
	}

	m_ID = glCreateProgram();
	glAttachShader(m_ID, vertex);
	glAttachShader(m_ID, fragment);
	glLinkProgram(m_ID);
	glGetProgramiv(m_ID, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(m_ID, 512, nullptr, infoLog);
		std::cout << "ERROR: SHADER::\"PROGRAM LINKING FAILED\"\n";
	}

	glDeleteShader(vertex);
	glDeleteShader(fragment);
};

void Shader::use() const
{
	glUseProgram(m_ID);
};

template<typename T>
void Shader::setUniform(const std::string& name, const T& value) const
{
	static_assert(false);
}

template<>
void Shader::setUniform(const std::string& name, const unsigned int& value) const
{
	if (glGetUniformLocation(m_ID, name.c_str()) == -1)
	{
		std::cout << "ERROR: There is no such a uniform named " << name.c_str() << " in the shader\n";
		return;
	}
	glUniform1ui(glGetUniformLocation(m_ID, name.c_str()), value);
};

template<>
void Shader::setUniform(const std::string& name, const int& value) const
{
	if (glGetUniformLocation(m_ID, name.c_str()) == -1)
	{
		std::cout << "ERROR: There is no such a uniform named " << name.c_str() << " in the shader\n";
		return;
	}
	glUniform1i(glGetUniformLocation(m_ID, name.c_str()), value);
};

template<>
void Shader::setUniform(const std::string& name, const float& value) const
{
	if (glGetUniformLocation(m_ID, name.c_str()) == -1)
	{
		std::cout << "ERROR: There is no such a uniform named " << name.c_str() << " in the shader\n";
		return;
	}
	glUniform1f(glGetUniformLocation(m_ID, name.c_str()), value);
};

template<>
void Shader::setUniform(const std::string& name, const glm::vec4& value) const
{
	if (glGetUniformLocation(m_ID, name.c_str()) == -1)
	{
		std::cout << "ERROR: There is no such a uniform named " << name.c_str() << " in the shader\n";
		return;
	}
	glUniform4f(glGetUniformLocation(m_ID, name.c_str()), value.x, value.y, value.z, value.w);
};

template<>
void Shader::setUniform(const std::string& name, const glm::mat4& value) const
{
	if (glGetUniformLocation(m_ID, name.c_str()) == -1)
	{
		std::cout << "ERROR: There is no such a uniform named " << name.c_str() << " in the shader\n";
		return;
	}
	glUniformMatrix4fv(glGetUniformLocation(m_ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
};

template<>
void Shader::setUniform(const std::string& name, const glm::vec3& value) const
{
	if (glGetUniformLocation(m_ID, name.c_str()) == -1)
	{
		std::cout << "ERROR: There is no such a uniform named " << name.c_str() << " in the shader\n";
		return;
	}
	glUniform3f(glGetUniformLocation(m_ID, name.c_str()), value.x, value.y, value.z);
};










#endif
