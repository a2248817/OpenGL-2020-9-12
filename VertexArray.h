#ifndef VERTEX_ARRAY_H
#define VERTEX_ARRAY_H

#include <string>

#include "ElementBuffer.h"
#include "VertexBuffer.h"

class VertexArray
{
private:
	struct Attribute
	{
		std::string name;
		unsigned int size;
	};
	std::vector<Attribute> m_Attributes;
	unsigned int		   m_ID;
	unsigned int		   m_VertexSize{ 0 };
	void setAttribute() const;
	void bindAll() const;
	void unbindAll() const;
public:
	VertexBuffer		   m_VB;
	ElementBuffer		   m_EB;
	VertexArray(const std::vector<float>& vb_data, const std::vector<unsigned int>& eb_data = {});
	VertexArray(const VertexArray& va);
	VertexArray(const VertexBuffer& vb, const ElementBuffer& eb);
	VertexArray(VertexArray&& va) noexcept;
	~VertexArray();
	void bind() const;
	void unbind() const;
	void addAttribute(const std::string& name, const unsigned int size_in_float);
};

VertexArray::VertexArray(const std::vector<float>& vb_data, const std::vector<unsigned int>& eb_data)
	:m_VB(vb_data), m_EB(eb_data)
{
	glGenVertexArrays(1, &m_ID);
	bindAll();
	unbindAll();
};

VertexArray::VertexArray(const VertexArray& va)
	:m_Attributes(va.m_Attributes), m_VertexSize(va.m_VertexSize), m_VB(va.m_VB), m_EB(va.m_EB)
{
	glGenVertexArrays(1, &m_ID);
	bindAll();
	setAttribute();
	unbindAll();
};

VertexArray::VertexArray(const VertexBuffer& vb, const ElementBuffer& eb)
	:m_VB(vb), m_EB(eb)
{
	glGenVertexArrays(1, &m_ID);
	bindAll();
	unbindAll();
};

VertexArray::VertexArray(VertexArray&& va) noexcept
	:m_Attributes(std::move(va.m_Attributes)), m_VertexSize(va.m_VertexSize), m_VB(std::move(va.m_VB)), m_EB(std::move(va.m_EB))
{
	glGenVertexArrays(1, &m_ID);
	bindAll();
	setAttribute();
	unbindAll();
};

VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &m_ID);
};

void VertexArray::bind() const
{
	glBindVertexArray(m_ID);
};

void VertexArray::unbind() const
{
	glBindVertexArray(0);
};

void VertexArray::setAttribute() const
{
	m_VB.bind();
	unsigned int offset{ 0 }, index{ 0 };
	for (auto& attri : m_Attributes)
	{
		glEnableVertexAttribArray(index);
		glVertexAttribPointer(index++, attri.size, GL_FLOAT, GL_FALSE, m_VertexSize * sizeof(float), reinterpret_cast<void*>(offset * sizeof(float)));
		offset += attri.size;
	}
	m_VB.unbind();
};

void VertexArray::addAttribute(const std::string& name, const unsigned int size_in_float)
{
	m_VertexSize += size_in_float;
	m_Attributes.emplace_back(Attribute{ name, size_in_float });
	setAttribute();
};

void VertexArray::bindAll() const
{
	bind();
	m_VB.bind();
	m_EB.bind();
};

void VertexArray::unbindAll() const
{
	unbind();
	m_VB.unbind();
	m_EB.unbind();
};

#endif
