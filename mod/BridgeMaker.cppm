export module BridgeMaker;


export import bm.config;
export import bm.app;
export import bm.graphic;

export import bm.layer.base;
export import bm.layer.imgui;

export import bm.event;

export import bm.log;

export import bm.input;
export import bm.window;
export import bm.cursor;

export import bm.utility;

export import bm.assert;

export import bm.gfx.camera;
export import bm.camerainp;

namespace bm
{
	export void init()
	{
		log::Log::init();
	}

}