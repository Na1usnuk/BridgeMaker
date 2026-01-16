module bm.gfx:mesh;

import :mesh;

namespace bm::gfx
{
	void Mesh::setIndexData(std::span<unsigned int> data, Usage usage)
	{
		m_index_data.reset();
		m_index_data.emplace(data.cbegin(), data.cend());
		m_index_usage = usage;
		m_version.index++;
	}

	std::span<const std::byte> Mesh::getVertexData() const noexcept
	{
		return m_vertex_data;
	}

	std::optional<std::span<const unsigned int>> Mesh::getIndexData() const noexcept
	{
		if (m_index_data)
		{
			std::span<const unsigned int> ibo(m_index_data->cbegin(), m_index_data->cend());
			return { ibo };
		}
		return std::nullopt;
	}

	Handle<VertexLayout> Mesh::getVertexLayout() const noexcept
	{
		return m_vertex_layout;
	}

	void Mesh::setVertexLayout(const Handle<VertexLayout> handler) noexcept
	{
		m_vertex_layout = handler;
		m_version.layout++;
	}

	void Mesh::clearVertexData() noexcept
	{
		m_vertex_data.clear();
		m_vertex_data.shrink_to_fit();
	}

	void Mesh::clearIndexData() noexcept
	{
		m_index_data.reset();
	}

	bm::observer_ptr<std::byte> Mesh::dataVertex() noexcept
	{
		m_version.vertex++; // Assuming user will modify data
		return m_vertex_data.data();
	}

	bm::observer_ptr<unsigned int> Mesh::dataIndex() noexcept
	{
		if (m_index_data)
		{
			m_version.index++; // Assuming user will modify data
			return m_index_data->data();
		}
		return nullptr;
	}

	std::size_t Mesh::sizeVertex() const noexcept
	{
		return m_vertex_data.size();
	}

	std::optional<std::size_t> Mesh::sizeIndex() const noexcept
	{
		if (m_index_data)
			return m_index_data->size();
		return std::nullopt;
	}
}