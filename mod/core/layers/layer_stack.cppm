export module bm.layer.stack;

import bm.layer.base;

import std;

namespace bm
{

export class LayerStack
{
public:

	using ptr_t = std::shared_ptr<Layer>;
	using raw_ptr_t = Layer*;

public:

	LayerStack() : m_inserter(m_layers.begin()) {}

	void pushLayer(ptr_t layer)
	{
		m_inserter = m_layers.emplace(m_inserter, layer);
		layer->onAttach();
	}

	void pushOverlay(ptr_t overlay)
	{
		m_layers.emplace_back(overlay);
		overlay->onAttach();
	}


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


}