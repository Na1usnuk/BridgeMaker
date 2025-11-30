export module bm.config;

// You willing to manually change any setting if needed

namespace bm::config
{
	export enum class Platform
	{
		Windows,
		Linux,
		MacOS
	};

	consteval Platform detect_platform()
	{
		#if defined(_WIN32)
		return Platform::Windows;
		#elif defined(__linux__)
		return Platform::Linux;
		#elif defined(__APPLE__)
		return Platform::MacOS;
		#else
		#error "Unsupported platform"
		#endif
	}

	export constexpr Platform platform = detect_platform();

	export constexpr bool is_windows = (platform == Platform::Windows);
	export constexpr bool is_linux   = (platform == Platform::Linux);
	export constexpr bool is_macos   = (platform == Platform::MacOS);

	export enum class BuildType
	{
		Debug,
		Release,
		Shipping
	};

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

	export constexpr BuildType build_type = detect_build_type();

	export constexpr bool is_debug    = (build_type == BuildType::Debug);
	export constexpr bool is_release  = (build_type == BuildType::Release);
	export constexpr bool is_shipping = (build_type == BuildType::Shipping);

	export constexpr bool enable_verify    = is_debug;
	export constexpr bool enable_imgui	   = true;//is_debug;
	export constexpr bool enable_profiling = is_debug;

	export constexpr bool enable_logging   = is_debug || is_release;

	export enum class GraphicsAPI
	{
		OpenGL,
		Vulkan,
		DirectX12,
		Metal
	};

	export constexpr GraphicsAPI graphics_api = GraphicsAPI::OpenGL; // Only OpenGL is supported for now

	// Do not set actual version, but use it as target when its time to choose. 
	// Consult bm::gfx::Context to get runtime version of API
	export constexpr int graphics_api_target_version = 0; // Set to 0 to get max available version

	export enum class Compiler
	{
		Unknown,
		MSVC,
		Clang,
		GCC
	};

	consteval Compiler detect_compiler()
	{
		#if defined(_MSC_VER)
			return Compiler::MSVC;
		#elif defined(__clang__)
			return Compiler::Clang;
		#elif defined(__GNUC__)
			return Compiler::GCC;
		#else
			//#error "Unsupported compiler"
			return Compiler::Unknown;
		#endif
	}

	export constexpr Compiler compiler = detect_compiler();

	export constexpr bool is_msvc = (compiler == Compiler::MSVC);
	export constexpr bool is_clang = (compiler == Compiler::Clang);
	export constexpr bool is_gcc = (compiler == Compiler::GCC);

}