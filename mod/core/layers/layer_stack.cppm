export module bm.layer.stack;

import bm.layer.base;
import bm.traits;

import std;

namespace bm
{

export class LayerStack
{
public:

	Traits<Layer>::OPtr pushLayer(Traits<Layer>::Ptr layer)
	{
		auto* raw = layer.get();
		layer->onAttach();
		m_layers.emplace(m_layers.begin() + m_insert_index++, std::move(layer));
		return { raw };
	}

	Traits<Layer>::OPtr pushOverlay(Traits<Layer>::Ptr overlay)
	{
		auto* raw = overlay.get();
		overlay->onAttach();
		m_layers.emplace_back(std::move(overlay));
		return { raw };
	}

	void popOverlay(Traits<Layer>::OPtr overlay)
	{
		auto it = findOverlayByPtr(overlay);
		if (it != m_layers.end())
		{
			(*it)->onDetach();
			m_layers.erase(it);
		}
	}

	void popOverlay(std::string_view name)
	{
		auto it = findOverlayByName(name);
		if (it != m_layers.end())
		{
			(*it)->onDetach();
			m_layers.erase(it);
		}
	}

	void popLayer(Traits<Layer>::OPtr layer)
	{
		auto it = findLayerByPtr(layer);
		if (it != m_layers.end())
		{
			(*it)->onDetach();
			m_layers.erase(it);
			--m_insert_index;
		}
	}

	void popLayer(std::string_view name)
	{
		auto it = findLayerByName(name);
		if (it != m_layers.end())
		{
			(*it)->onDetach();
			m_layers.erase(it);
			--m_insert_index;
		}
	}

	Traits<Layer>::OPtr getOverlay(std::string_view name)
	{
		auto it = findOverlayByName(name);
		return (it == m_layers.end() ?  nullptr : it->get() );
	}

	Traits<Layer>::OPtr getLayer(std::string_view name)
	{
		auto it = findLayerByName(name);
		return (it == m_layers.end() ?  nullptr : it->get() );
	}

	std::vector<Traits<Layer>::Ptr>::iterator begin() { return m_layers.begin(); }
	std::vector<Traits<Layer>::Ptr>::iterator end() { return m_layers.end(); }

private:

	// To avoid code duplication
	std::vector<Traits<Layer>::Ptr>::iterator findOverlayByName(std::string_view name)
	{
		return std::find_if(m_layers.begin() + m_insert_index, m_layers.end(), [name](Traits<Layer>::Ptr& _) { return name == _->getName(); });
	}

	std::vector<Traits<Layer>::Ptr>::iterator findLayerByName(std::string_view name)
	{
		return std::find_if(m_layers.begin(), m_layers.begin() + m_insert_index, [name](Traits<Layer>::Ptr& _) { return name == _->getName(); });
	}

	std::vector<Traits<Layer>::Ptr>::iterator findOverlayByPtr(Traits<Layer>::OPtr overlay)
	{
		return std::find_if(m_layers.begin() + m_insert_index, m_layers.end(), [&overlay](Traits<Layer>::Ptr& _) { return _.get() == overlay.get(); });
	}

	std::vector<Traits<Layer>::Ptr>::iterator findLayerByPtr(Traits<Layer>::OPtr layer)
	{
		return std::find_if(m_layers.begin(), m_layers.begin() + m_insert_index, [&layer](Traits<Layer>::Ptr& _) { return _.get() == layer.get(); });
	}

private:

	std::vector<Traits<Layer>::Ptr> m_layers;
	std::vector<Traits<Layer>::Ptr>::size_type m_insert_index = 0;
};


}