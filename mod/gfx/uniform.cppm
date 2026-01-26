export module bm.gfx:uniform;

import :utility;

import std;
import glm;

namespace bm::gfx
{
	
	export using UniformType = DataType;

	export using UniformData = std::variant
		<
			float,
			int,
			//glm::vec2,
			glm::vec3,
			glm::vec4,
			//glm::mat3,
			glm::mat4
		>;

	export struct UniformBinding 
	{
		std::string name;
		UniformData data;
		std::uint32_t version = 0;
	};

	export class Bindings
	{
	public:

		using Version = std::uint64_t;

	public:

		void set(std::string name, UniformData data)
		{
			auto it = std::ranges::find(m_bindings, name, &UniformBinding::name);
			if (it not_eq m_bindings.end())
			{
				it->data = std::move(data);
				it->version++;
			}
			else
				m_bindings.emplace_back(UniformBinding{std::move(name), std::move(data)});
			m_version++;
		}

		void remove(std::string_view name)
		{
			auto [first, last] = std::ranges::remove(m_bindings, name, &UniformBinding::name);
			if (first not_eq last) 
			{
				m_bindings.erase(first, last);
				m_version++;
			}
		}

		bool contains(std::string_view name) const noexcept 
		{
			return std::ranges::find(m_bindings, name, &UniformBinding::name) not_eq m_bindings.end();
		}

		std::optional<std::reference_wrapper<UniformBinding>> get(std::string_view name)
		{
			auto it = std::ranges::find(m_bindings, name, &UniformBinding::name);
			if (it not_eq m_bindings.end())
				return { *it };
			else
				return std::nullopt;
		}

		std::span<const UniformBinding> getBindings() const noexcept { return { m_bindings }; }
		Version getVersion() const noexcept { return m_version; }

	private:

		std::vector<UniformBinding> m_bindings;
		Version m_version = 0;
	};
}