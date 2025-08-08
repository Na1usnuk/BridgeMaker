#pragma once


class IndexBuffer
{
public:

	IndexBuffer(const unsigned int* data, unsigned long long count);
	~IndexBuffer();

	void destroy();

	void bind() const;
	void unbind() const;

	unsigned int count() const { return m_count; }

private:

	unsigned int m_id;
	unsigned long long m_count;
};

