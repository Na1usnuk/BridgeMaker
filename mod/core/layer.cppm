export module bm.core:layer;

import std;

import :traits;

namespace bm 
{
	
	export class Layer
	{
	public:
	
		Layer(std::string_view name = "Layer")
			: m_name(name)
		{}
	
		std::string_view getName() const { return m_name; }
		void setName(std::string_view name) { m_name = name; }
	
		Layer() = default;
		Layer(const Layer&) = delete;
		Layer& operator=(const Layer&) = delete;
		Layer(Layer&&) = delete;
		Layer& operator=(Layer&&) = delete;
	
		virtual ~Layer() {}
	
		bool isEnabled() const { return m_enabled; }
		bool isDisabled() const { return not isEnabled(); }
		void enable(bool enable = true) { m_enabled = enable; }
		void disable(bool disable = true) { enable(not disable); }
	
		virtual void onAttach() {}
		virtual void onDetach() {}
		virtual void onUpdate(float delta_time) {}
	
		template<class L, typename... Args>
			requires std::is_base_of_v<Layer, L>
		static Traits<L>::Ptr make(Args&&... args) { return std::make_unique<L>(std::forward<Args>(args)...); }
	
	private:
	
		bool m_enabled = true;
	
		std::string m_name;
	
	};


	export class LayerStack
	{
	public:

		Traits<Layer>::OPtr pushLayer(Traits<Layer>::Ptr layer);
		Traits<Layer>::OPtr pushOverlay(Traits<Layer>::Ptr overlay);

		void popOverlay(Traits<Layer>::OPtr overlay);
		void popOverlay(std::string_view name);

		void popLayer(Traits<Layer>::OPtr layer);
		void popLayer(std::string_view name);

		Traits<Layer>::OPtr getOverlay(std::string_view name);
		Traits<Layer>::OPtr getLayer(std::string_view name);

		std::vector<Traits<Layer>::Ptr>::iterator begin() { return m_layers.begin(); }
		std::vector<Traits<Layer>::Ptr>::iterator end() { return m_layers.end(); }

	private:

		// Helpers
		std::vector<Traits<Layer>::Ptr>::iterator findOverlayByName(std::string_view name);
		std::vector<Traits<Layer>::Ptr>::iterator findLayerByName(std::string_view name);
		std::vector<Traits<Layer>::Ptr>::iterator findOverlayByPtr(Traits<Layer>::OPtr overlay);
		std::vector<Traits<Layer>::Ptr>::iterator findLayerByPtr(Traits<Layer>::OPtr layer);

	private:

		std::vector<Traits<Layer>::Ptr> m_layers;
		std::vector<Traits<Layer>::Ptr>::size_type m_insert_index = 0;
	};
	

}