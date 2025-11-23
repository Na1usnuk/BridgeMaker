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

export import bm.verify;
export import bm.assetmanager;

export import bm.gfx.camera;
export import bm.gfx.camera.screen;
export import bm.camerainp;

export import bm.traits;

namespace bm
{
	export void init()
	{
		log::Log::init();
	}

}