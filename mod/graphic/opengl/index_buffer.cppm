export module bm.gfx.buffer.index;

namespace bm::gfx
{

export class IndexBuffer
{
public:

	IndexBuffer(const unsigned int* data, unsigned long long count);
	~IndexBuffer();
	IndexBuffer(const IndexBuffer&) = delete;
	IndexBuffer& operator=(const IndexBuffer&) = delete;
	IndexBuffer(IndexBuffer&& oth) noexcept;
	IndexBuffer& operator=(IndexBuffer&& oth) noexcept;

	void destroy();

	void bind() const;
	void unbind() const;

	unsigned int count() const { return m_count; }

private:

	unsigned int m_id = 0;
	unsigned long long m_count = 0;
};

}

