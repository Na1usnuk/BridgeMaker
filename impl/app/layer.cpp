module bm.app:layer;
import :layer;

import std;

namespace bm
{
	/*LayerStack::LayerHandle LayerStack::pushLayer(Layer&& layer)
	{
		layer.onAttach();
		LayerHandle handle(m_insert_index);
		m_layers.emplace(m_layers.begin() + m_insert_index++, std::move(layer));
		return handle;
	}

	LayerStack::LayerHandle LayerStack::pushOverlay(Layer&& overlay)
	{
		overlay.onAttach();
		LayerHandle handle(m_layers.size());
		m_layers.emplace_back(std::move(overlay));
		return handle;
	}

	void LayerStack::popOverlay(LayerHandle overlay)
	{
		auto it = findOverlayByPtr(overlay);
		if (it != m_layers.end())
		{
			(*it)->onDetach();
			m_layers.erase(it);
		}

	}

	void LayerStack::popOverlay(std::string_view name)
	{
		auto it = findOverlayByName(name);
		if (it != m_layers.end())
		{
			(*it)->onDetach();
			m_layers.erase(it);
		}
	}

	void LayerStack::popLayer(Traits<Layer>::OPtr layer)
	{
		auto it = findLayerByPtr(layer);
		if (it != m_layers.end())
		{
			(*it)->onDetach();
			m_layers.erase(it);
			--m_insert_index;
		}
	}

	void LayerStack::popLayer(std::string_view name)
	{
		auto it = findLayerByName(name);
		if (it != m_layers.end())
		{
			(*it)->onDetach();
			m_layers.erase(it);
			--m_insert_index;
		}
	}

	Traits<Layer>::OPtr LayerStack::getOverlay(std::string_view name)
	{
		auto it = findOverlayByName(name);
		return (it == m_layers.end() ? nullptr : it->get());
	}

	Traits<Layer>::OPtr LayerStack::getLayer(std::string_view name)
	{
		auto it = findLayerByName(name);
		return (it == m_layers.end() ? nullptr : it->get());
	}

	std::vector<Traits<Layer>::Ptr>::iterator LayerStack::findOverlayByName(std::string_view name)
	{
		return std::find_if(m_layers.begin() + m_insert_index, m_layers.end(), [name](Traits<Layer>::Ptr& _) { return name == _->getName(); });
	}

	std::vector<Traits<Layer>::Ptr>::iterator LayerStack::findLayerByName(std::string_view name)
	{
		return std::find_if(m_layers.begin(), m_layers.begin() + m_insert_index, [name](Traits<Layer>::Ptr& _) { return name == _->getName(); });
	}

	std::vector<Traits<Layer>::Ptr>::iterator LayerStack::findOverlayByPtr(Traits<Layer>::OPtr overlay)
	{
		return std::find_if(m_layers.begin() + m_insert_index, m_layers.end(), [&overlay](Traits<Layer>::Ptr& _) { return _.get() == overlay.get(); });
	}

	std::vector<Traits<Layer>::Ptr>::iterator LayerStack::findLayerByPtr(Traits<Layer>::OPtr layer)
	{
		return std::find_if(m_layers.begin(), m_layers.begin() + m_insert_index, [&layer](Traits<Layer>::Ptr& _) { return _.get() == layer.get(); });
	}*/
}