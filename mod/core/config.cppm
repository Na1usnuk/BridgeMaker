export module bm.core:config;
import std;

// ============================================================================
// CONFIG MODULE
// ============================================================================
// This module provides compile-time configuration detection and user-tunable
// settings. Most settings auto-detect optimal values, but can be manually
// overridden if needed. All values are constexpr for zero runtime overhead.
// ============================================================================

namespace bm::gfx
{
	// ------------------------------------------------------------------------
	// Graphics API Version Representation
	// ------------------------------------------------------------------------
	export struct Version
	{
		int major, minor;
		constexpr bool operator==(const Version&) const = default;
		constexpr auto operator<=>(const Version&) const = default;
		constexpr operator int() const { return major * 10 + minor; }
	};

	// ------------------------------------------------------------------------
	// Supported OpenGL Versions (in order of preference, newest first)
	// ------------------------------------------------------------------------
	// When creating contexts, the engine will iterate through this list
	// starting from target_version until a working context is created.
	export constexpr std::array<Version, 8> versions =
	{
		Version{4, 6},
		Version{4, 5},
		Version{4, 4},
		Version{4, 3},
		Version{4, 2},
		Version{4, 1},
		Version{4, 0},
		Version{3, 3} 
	};

	// ------------------------------------------------------------------------
	// Graphics API Selection
	// ------------------------------------------------------------------------
	export enum class API
	{
		OpenGL, 
		Vulkan,    // Not supported
		DirectX12, // Not supported
		Metal      // Not supported
	};
}

namespace bm::config
{
	// ========================================================================
	// GRAPHICS CONFIGURATION
	// ========================================================================
	namespace gfx
	{
		// --------------------------------------------------------------------
		// Active Graphics API
		// --------------------------------------------------------------------
		// Currently only OpenGL is implemented.
		export constexpr ::bm::gfx::API api = ::bm::gfx::API::OpenGL;

		// --------------------------------------------------------------------
		// Target OpenGL Version
		// --------------------------------------------------------------------
		// This is the PREFERRED version, not the guaranteed version.
		// At runtime, the engine will attempt to create a context with this
		// version, falling back to older versions if necessary.
		// 
		// The actual runtime version can be queried via bm::gfx::Context.
		// Set this to match your shader requirements or leave at newest (4.6)
		// for maximum feature availability.
		export constexpr ::bm::gfx::Version target_version = ::bm::gfx::versions[0];
	}

	// ========================================================================
	// PLATFORM DETECTION
	// ========================================================================
	export enum class Platform
	{
		Windows,
		Linux,  
		MacOS   
	};

	export enum class Compiler
	{
		Unknown,
		MSVC,   
		Clang,  
		GCC     
	};

	export enum class BuildType
	{
		Debug,   
		Release, 
		Shipping 
	};

	// ------------------------------------------------------------------------
	// Platform Detection
	// ------------------------------------------------------------------------
	consteval Platform detect_platform()
	{
#if defined(_WIN32)
		return Platform::Windows;
#elif defined(__linux__)
		return Platform::Linux;
#elif defined(__APPLE__)
		return Platform::MacOS;
#else
#error "Unsupported platform detected. Supported: Windows, Linux, MacOS"
#endif
	}

	// ------------------------------------------------------------------------
	// Build Type Detection
	// ------------------------------------------------------------------------
	consteval BuildType detect_build_type()
	{
#if defined(NDEBUG)
#if defined(SHIPPING)
		return BuildType::Shipping;
#else
		return BuildType::Release;
#endif
#else
		return BuildType::Debug;
#endif
	}

	// ------------------------------------------------------------------------
	// Compiler Detection
	// ------------------------------------------------------------------------
	consteval Compiler detect_compiler()
	{
#if defined(_MSC_VER)
		return Compiler::MSVC;
#elif defined(__clang__)
		return Compiler::Clang;
#elif defined(__GNUC__)
		return Compiler::GCC;
#else
		// Note: Unknown compiler - may work but is untested
		return Compiler::Unknown;
#endif
	}

	// ------------------------------------------------------------------------
	// Detected Configuration (Auto-detected, do not modify)
	// ------------------------------------------------------------------------
	export constexpr Platform  platform   = detect_platform();
	export constexpr BuildType build_type = detect_build_type();
	export constexpr Compiler  compiler   = detect_compiler();

	// ------------------------------------------------------------------------
	// Platform Convenience Flags
	// ------------------------------------------------------------------------
	export constexpr bool is_windows = (platform == Platform::Windows);
	export constexpr bool is_linux =   (platform == Platform::Linux);
	export constexpr bool is_macos =   (platform == Platform::MacOS);

	// ------------------------------------------------------------------------
	// Build Type Convenience Flags
	// ------------------------------------------------------------------------
	export constexpr bool is_debug    = (build_type == BuildType::Debug);
	export constexpr bool is_release  = (build_type == BuildType::Release);
	export constexpr bool is_shipping = (build_type == BuildType::Shipping);

	// ------------------------------------------------------------------------
	// Compiler Convenience Flags
	// ------------------------------------------------------------------------
	export constexpr bool is_msvc  = (compiler == Compiler::MSVC);
	export constexpr bool is_clang = (compiler == Compiler::Clang);
	export constexpr bool is_gcc   = (compiler == Compiler::GCC);

	// ========================================================================
	// FEATURE TOGGLES (User-configurable)
	// ========================================================================

	// ------------------------------------------------------------------------
	// Runtime Assertions and Verification
	// ------------------------------------------------------------------------
	// Enables verify() and assert-like checks throughout the codebase.
	// Recommended: Enable in Debug, disable in Release/Shipping for performance
	export constexpr bool enable_verify = is_debug;

	// ------------------------------------------------------------------------
	// ImGui Debug Interface
	// ------------------------------------------------------------------------
	// Enables the Dear ImGui debug UI for runtime inspection and tweaking.
	// Useful for: profiler overlay, shader hot-reload UI, debug visualizations
	// Disable in shipping builds to avoid exposing debug features to users.
	export constexpr bool enable_imgui = is_debug;

	// ------------------------------------------------------------------------
	// Performance Profiling
	// ------------------------------------------------------------------------
	// Enables instrumentation for performance profiling (CPU/GPU timers).
	// Small overhead in Debug, minimal in Release. Disable in Shipping.
	export constexpr bool enable_profiling = is_debug;

	// ------------------------------------------------------------------------
	// Logging System
	// ------------------------------------------------------------------------
	// Enables the logging system for diagnostics and error reporting.
	// Recommended: Enable in Debug/Release, disable in Shipping (or file-only)
	export constexpr bool enable_logging = is_debug || is_release;
}