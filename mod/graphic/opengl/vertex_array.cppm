export module bm.gfx.vertexarray;



namespace bm::gfx
{

class VertexBuffer;
class VertexBufferLayout;

export class VertexArray
{
private:

public:

	VertexArray();
	~VertexArray();
	VertexArray(const VertexArray&) = delete;
	VertexArray& operator=(const VertexArray&) = delete;
	VertexArray(VertexArray&& oth) noexcept;
	VertexArray& operator=(VertexArray&& oth) noexcept;

	void addVertexBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);

	void bind() const;
	void unbind() const;
	void destroy();

private:

	unsigned int m_id = 0;

};

}
