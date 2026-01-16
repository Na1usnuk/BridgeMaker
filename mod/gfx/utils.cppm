export module bm.gfx:utility;

import bm.core;
import std;

namespace bm::gfx
{

	// Used to specify the expected usage pattern of the data store
	export enum class Usage
	{
		Const,	 // For immutable data
		Static,  // Data changes rarelly
		Dynamic, // Data can change sometimes
		Stream,  // Data changes frequently 
	};
	
}



