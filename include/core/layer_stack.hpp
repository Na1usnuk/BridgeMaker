#pragma once 

#include "core.hpp"
#include "layer.hpp"

#include <memory>
#include <vector>
#include <algorithm>
#include <utility>

BM_START

/*
template<typename ...AllLayerTypeNames>
class LayerStack_that_works_like_shit
{
public:

	using variant_t = std::variant<AllLayerTypeNames...>;
	static constexpr std::size_t count = sizeof...(AllLayerTypeNames);
	using data_t = std::array<variant_t, count>;

public:

	LayerStack() 
		: m_layers(defaultInit(std::index_sequence_for<AllLayerTypeNames...>{}))
	{
	}

	template<typename LayerTypeToDisable>
	void disable()
	{
		auto it = std::find_if(begin(), end(), [](variant_t v) {return std::holds_alternative<LayerTypeToDisable>(v); });
		if (it != end())
			std::visit([](auto& obj) {obj.disable(); }, *it);
	}

	template<typename LayerTypeToDisable>
	void enable()
	{
		auto it = std::find_if(begin(), end(), [](variant_t v) {return std::holds_alternative<LayerTypeToDisable>(v); });
		if (it != end())
			std::visit([](auto& obj) {obj.enable(); }, *it);
	}

	data_t::iterator begin() { return m_layers.begin(); }
	data_t::iterator end() { return m_layers.end(); }

private:

	template<size_t... indices>
	inline static data_t defaultInit(std::index_sequence<indices...>)
	{
		return data_t{ variant_t{std::in_place_type<AllLayerTypeNames>}... };
	}

private:

	data_t m_layers;

};
*/


class LayerStack
{
public:

	using ptr_t = std::unique_ptr<Layer>;
	using raw_ptr_t = Layer*;

public:

	LayerStack() { m_inserter = m_layers.begin(); }

	void pushLayer(raw_ptr_t layer)
	{
		m_inserter = m_layers.emplace(m_inserter, layer);
		layer->onAttach();
	}

	void pushOverlay(raw_ptr_t overlay)
	{
		m_layers.emplace_back(overlay);
		overlay->onAttach();
	}

	//void pushLayer(ptr_t layer)
	//{
	//	m_inserter = m_layers.insert(m_inserter, layer);
	//}

	//void pushOverlay(ptr_t overlay)
	//{
	//	m_layers.push_back(overlay);
	//}

	void popOverlay(ptr_t overlay)
	{
		auto finded = std::find(m_layers.begin(), m_layers.end(), overlay);
		if (finded != m_layers.end())
			m_layers.erase(finded);
		overlay->onDetach();
	}

	void popLayer(ptr_t layer) 
	{
		auto finded = std::find(m_layers.begin(), m_layers.end(), layer);
		if (finded != m_layers.end())
		{
			m_layers.erase(finded);
			--m_inserter;
		}
		layer->onDetach();
	}


	std::vector<ptr_t>::iterator begin() { return m_layers.begin(); }
	std::vector<ptr_t>::iterator end() { return m_layers.end(); }


private:

	std::vector<ptr_t> m_layers;
	std::vector<ptr_t>::iterator m_inserter;
};


BM_END