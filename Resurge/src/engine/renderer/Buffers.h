#pragma once

#include <glad/glad.h>
#include <vector>
#include <engine/core.h>

class VertexBuffer
{
public:
	// default cons
	VertexBuffer() {}

	VertexBuffer(const void* data, unsigned int size);
	~VertexBuffer();

	void Bind() const;
	void Unbind() const;

private:
	unsigned int m_RendererID;
};

class IndexBuffer
{
public:
	// default cons
	IndexBuffer() {}

	IndexBuffer(const unsigned int *data, unsigned int count);
	~IndexBuffer();
	
	void Bind() const;
	void Unbind() const;

	inline unsigned int GetCount() const { return m_Count;  }

private:
	unsigned int m_RendererID;
	unsigned int m_Count;
};

struct VertexBufferElement
{
	unsigned int type;
	unsigned int count;
	unsigned char normalized;

	static unsigned int GetSizeofType(unsigned int type)
	{
		switch (type)
		{
		case GL_FLOAT:			return 4;
		case GL_UNSIGNED_INT:	return 4;
		case GL_UNSIGNED_BYTE:	return 1;
		}
		AB_ASSERT(false, "Type not found");
		return 0;
	}
};

class VertexBufferLayout
{
public:
	VertexBufferLayout()
		: m_Stride(0) {}

	template<typename T>
	void Push(unsigned int count)
	{
		static_assert(false);
	}

	template<>
	void Push<float>(unsigned int count)
	{
		m_Elements.push_back({ GL_FLOAT, count, GL_FALSE });
		m_Stride += count * VertexBufferElement::GetSizeofType(GL_FLOAT);
	}

	template<>
	void Push<unsigned int>(unsigned int count)
	{
		m_Elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
		m_Stride += count * VertexBufferElement::GetSizeofType(GL_UNSIGNED_INT);
	}

	template<>
	void Push<unsigned char>(unsigned int count)
	{
		m_Elements.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE });
		m_Stride += count * VertexBufferElement::GetSizeofType(GL_UNSIGNED_BYTE);
	}

	inline const std::vector<VertexBufferElement> GetElements() const { return m_Elements; }
	inline unsigned int GetStride() const { return m_Stride; }

private:
	std::vector<VertexBufferElement>  m_Elements;
	unsigned int m_Stride;
};

class VertexArray
{
public:
	VertexArray();
	~VertexArray();

	void AddBuffer(const VertexBuffer* vb, const VertexBufferLayout& layout);

	void Bind() const;
	void Unbind() const;

private:
	unsigned int m_RendererID;

};