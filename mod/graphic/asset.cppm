export module bm.gfx.asset;

import std;

import bm.traits;

namespace bm::gfx
{
	export class Asset
	{
	public:

		virtual ~Asset() {}

		bool isReady() const { return m_ready.load(); }



	private:

		std::atomic<bool> m_ready;

		Traits<Asset>::Ptr m_this;
		//std::future<Asset>

	};
}