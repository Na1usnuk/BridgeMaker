module bm.core:layer;
import :layer;
import :traits;

import std;

namespace bm
{
	Traits<Layer>::OPtr LayerStack::pushLayer(Traits<Layer>::Ptr layer)
	{
		auto* raw = layer.get();
		layer->onAttach();
		m_layers.emplace(m_layers.begin() + m_insert_index++, std::move(layer));
		return { raw };
	}

	Traits<Layer>::OPtr LayerStack::pushOverlay(Traits<Layer>::Ptr overlay)
	{
		auto* raw = overlay.get();
		overlay->onAttach();
		m_layers.emplace_back(std::move(overlay));
		return { raw };
	}

	void LayerStack::popOverlay(Traits<Layer>::OPtr overlay)
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
	}
}