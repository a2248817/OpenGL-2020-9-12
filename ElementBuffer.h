#ifndef ELEMENTBUFFER_H
#define ELEMENTBUFFER_H

#include <glad/glad.h>
#include <vector>

class ElementBuffer
{
private:
	unsigned int              m_ID;
	std::vector<unsigned int> m_Indices;
public:
	ElementBuffer(const std::vector<unsigned int>& indexData);
	ElementBuffer(const ElementBuffer& eb);
	ElementBuffer(ElementBuffer&& eb) noexcept;
	~ElementBuffer();
	void bind() const;
	void unbind() const;
	ElementBuffer& operator = (const ElementBuffer& eb);
};

ElementBuffer::ElementBuffer(const std::vector<unsigned int>& indexData)
	:m_Indices(indexData)
{
	glGenBuffers(1, &m_ID);
};

ElementBuffer::ElementBuffer(const ElementBuffer& eb)
	:m_Indices(eb.m_Indices)
{
	glGenBuffers(1, &m_ID);
};

ElementBuffer::ElementBuffer(ElementBuffer&& eb) noexcept
	:m_Indices(std::move(eb.m_Indices))
{
	glGenBuffers(1, &m_ID);
};

ElementBuffer::~ElementBuffer()
{
	glDeleteBuffers(1, &m_ID);
};

void ElementBuffer::bind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
	if(m_Indices.size())
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Indices.size() * sizeof(unsigned int), &m_Indices[0], GL_STATIC_DRAW);
};

void ElementBuffer::unbind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
};

ElementBuffer& ElementBuffer::operator = (const ElementBuffer& eb)
{
	m_Indices = eb.m_Indices;
	return *this;
};

#endif