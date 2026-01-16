export module bm.gfx:mesh;

import :buffer;
import :utility;

import bm.core;

import std;

namespace bm::gfx
{

	export class Mesh
	{
	public:
	
		enum class Topology
		{
			Triangles,
			Lines,
			Points
		};

		struct Version
		{
			std::size_t vertex = 0;
			std::size_t index = 0;
			std::size_t layout = 0;

			bool operator==(const Version& oth) const noexcept
			{
				return vertex == oth.vertex
					and index == oth.index
					and layout == oth.layout;
			}

			bool operator!=(const Version& oth) const noexcept
			{
				return not(*this == oth);
			}
		};

	public:

		Mesh(Topology topology = Topology::Triangles) :
			m_topology(topology)
		{}

		Mesh(const Mesh&) = delete;
		Mesh& operator=(const Mesh&) = delete;

		Mesh(Mesh&&) noexcept = default;
		Mesh& operator=(Mesh&&) noexcept = default;

		~Mesh() = default;
	
		template<std::ranges::contiguous_range R>
			requires std::is_trivially_copyable_v<std::ranges::range_value_t<R>>
		void setVertexData(const R& data, Usage usage = Usage::Const);
		// TODO: variadic index type
		void setIndexData(std::span<unsigned int> data, Usage usage = Usage::Const);

		std::span<const std::byte> getVertexData() const noexcept;
		std::optional<std::span<const unsigned int>> getIndexData() const noexcept;

		Handle<VertexLayout> getVertexLayout() const noexcept;
		void setVertexLayout(const Handle<VertexLayout> handler) noexcept;

		void clearVertexData() noexcept;
		void clearIndexData() noexcept;

		// (almost) Raw pointer getter
		bm::observer_ptr<std::byte> dataVertex() noexcept;
		bm::observer_ptr<unsigned int> dataIndex() noexcept;

		std::size_t sizeVertex() const noexcept;
		std::optional<std::size_t> sizeIndex() const noexcept;
	
		Topology getTopology() const noexcept { return m_topology; }
		void setTopology(Topology topology) noexcept { m_topology = topology; }

		Usage getVertexUsage() const noexcept { return m_vertex_usage; }
		void setVertexUsage(Usage usage) noexcept { m_vertex_usage = usage; }

		Usage getIndexUsage() const noexcept { return m_index_usage; }
		void setIndexUsage(Usage usage) noexcept { m_index_usage = usage; }

		const Version& getVersion() const noexcept { return m_version; }
	
	private:
	
		Topology m_topology;
		Version m_version;

		std::vector<std::byte> m_vertex_data;
		Handle<VertexLayout> m_vertex_layout;
		Usage m_vertex_usage;

		std::optional<std::vector<unsigned int>> m_index_data;
		Usage m_index_usage;

	};
	

	template<std::ranges::contiguous_range R>
		requires std::is_trivially_copyable_v<std::ranges::range_value_t<R>>
	void Mesh::setVertexData(const R& data, Usage usage)
	{
		std::size_t data_size = std::ranges::size(data) * sizeof(std::ranges::range_value_t<R>);
		m_vertex_data.resize(data_size);
		std::memcpy(m_vertex_data.data(), std::ranges::data(data), data_size);

		m_vertex_usage = usage;
		m_version.vertex++;
	}
	
}