#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include <glad/glad.h>

#include "stb_image.h"


class Texture
{
public:
	std::string  m_FilePath{"NULL"};
	unsigned int m_ID{};
public:
	Texture(const char* FilePath);
	void loadImageFile(const char* FilePath);
	static void flipY(bool condition);
	unsigned int bind(unsigned int textureUnit);
};

Texture::Texture(const char* filePath)
	:m_FilePath(filePath)
{
	glGenTextures(1, &m_ID);
	glBindTexture(GL_TEXTURE_2D, m_ID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	loadImageFile(filePath);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

void Texture::loadImageFile(const char* filePath)
{
	int width, height, channels;
	unsigned char* data{ stbi_load(filePath, &width, &height, &channels, 0) };
	GLenum format{};

	switch (channels)
	{
	case 1:	format = GL_RED;  break;
	case 3:	format = GL_RGB;  break;
	case 4:	format = GL_RGBA; break;
	}

	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "ERROR -> Texture.h: Texture failed to load at path: " << m_FilePath << '\n';
	stbi_image_free(data);
}

void Texture::flipY(bool flag)
{
	stbi_set_flip_vertically_on_load(flag);
}

unsigned int Texture::bind(unsigned int textureUnit)
{
	glActiveTexture(textureUnit);
	glBindTexture(GL_TEXTURE_2D, m_ID);
	return (textureUnit - GL_TEXTURE0);
}

#endif

