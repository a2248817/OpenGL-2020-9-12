#ifndef VERTEXBUFFER_H
#define VERTEXBUFFER_H

#include <glad/glad.h>
#include <vector>

class VertexBuffer
{
private:
	unsigned int           m_ID;
	std::vector<float>     m_Vertices;
public:
	VertexBuffer(const std::vector<float>& vertexData);
	VertexBuffer(const VertexBuffer& vb);
	VertexBuffer(VertexBuffer&& vb) noexcept;
	VertexBuffer(int);
	~VertexBuffer();
	void bind() const;
	void unbind() const;
	VertexBuffer& operator = (const VertexBuffer& vb);
};

VertexBuffer::VertexBuffer(const std::vector<float>& vertexData)
	:m_Vertices(vertexData)
{
	glGenBuffers(1, &m_ID);
};

VertexBuffer::VertexBuffer(const VertexBuffer& vb)
	:m_Vertices(vb.m_Vertices)
{
	glGenBuffers(1, &m_ID);
};

VertexBuffer::VertexBuffer(VertexBuffer&& vb) noexcept
	:m_Vertices(std::move(vb.m_Vertices))
{
	glGenBuffers(1, &m_ID);
};

VertexBuffer::VertexBuffer(int)
	:m_ID(0)
{
};

VertexBuffer::~VertexBuffer()
{
	glDeleteBuffers(1, &m_ID);
};

void VertexBuffer::bind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, m_ID);
	glBufferData(GL_ARRAY_BUFFER, m_Vertices.size() * sizeof(float), &m_Vertices[0], GL_STATIC_DRAW);
};

void VertexBuffer::unbind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
};

VertexBuffer& VertexBuffer::operator = (const VertexBuffer& vb)
{
	m_Vertices = vb.m_Vertices;
	return *this;
};

#endif