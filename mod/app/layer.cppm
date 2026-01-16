export module bm.app:layer;

import std;

import bm.core;
import :event;

namespace bm 
{
	
	export class Layer
	{
	public:
	
		Layer(std::string_view name = "Layer")
			: m_name(name)
		{}
	
		virtual ~Layer() {}
	
		Layer() = default;
		Layer(const Layer&) = delete;
		Layer& operator=(const Layer&) = delete;
		Layer(Layer&&) = delete;
		Layer& operator=(Layer&&) = delete;
	
		std::string_view getName() const { return m_name; }
		void setName(std::string_view name) { m_name = name; }
	
		bool isEnabled() const { return m_enabled; }
		void enable(bool enable = true) { m_enabled = enable; }
		void disable(bool disable = true) { enable(not disable); }
	
		virtual void onAttach() {}
		virtual void onDetach() {}
		virtual void onUpdate(float) {}
		virtual void onEvent(Event&) {}
	
	private:
	
		bool m_enabled = true;
	
		std::string m_name;
	
	};


	/*template<std::size_t LayerMaxCount = 10, std::size_t OverlayMaxCount = 5>
	export class LayerStack
	{
	public:

		using LayerHandle = Handle<Layer>;

	private:

		struct Slot
		{
			std::optional<Layer> layer;
			std::uint32_t generation = 0;
		};

	public:

		LayerHandle pushLayer(Layer&& layer);
		LayerHandle pushOverlay(Layer&& overlay);

		void popOverlay(LayerHandle overlay);
		void popLayer(LayerHandle layer);

		Layer& getOverlay(LayerHandle handle);
		Layer& getLayer(LayerHandle handle);

	private:

		std::array<Slot, LayerMaxCapacity + OverlayMaxCapacity> m_layers;


	private:

		std::vector<Layer> m_layers;
		std::vector<Layer>::size_type m_insert_index = 0;


	};*/
	

}