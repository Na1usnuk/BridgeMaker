export module bm.gfx:utility;

import bm.core;
import std;

namespace bm::gfx
{

	export enum class DataType
	{
		Float,
		Vec2,
		Vec3,
		Vec4,

		Double,
		DVec2,
		DVec3,
		DVec4,

		Int,
		IVec2,
		IVec3,
		IVec4,

		UInt,
		UVec2,
		UVec3,
		UVec4,

		Bool,
		BVec2,
		BVec3,
		BVec4,
	};

	// Used to specify the expected usage pattern of the data store
	export enum class Usage
	{
		Const,	 // For immutable data
		Static,  // Data changes rarelly
		Dynamic, // Data can change sometimes
		Stream,  // Data changes frequently 
	};
	
}



