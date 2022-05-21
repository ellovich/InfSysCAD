#pragma once

#include <memory>

#include "CAD/Core/PlatformDetection.h"

#ifdef INFSYS_DEBUG
	#if defined(INFSYS_PLATFORM_WINDOWS)
		#define INFSYS_DEBUGBREAK() __debugbreak()
	#elif defined(INFSYS_PLATFORM_LINUX)
		#include <signal.h>
		#define INFSYS_DEBUGBREAK() raise(SIGTRAP)
	#else
		#error "Platform doesn't support debugbreak yet!"
	#endif
	#define INFSYS_ENABLE_ASSERTS
#else
	#define INFSYS_DEBUGBREAK()
#endif

#define INFSYS_EXPAND_MACRO(x) x
#define INFSYS_STRINGIFY_MACRO(x) #x

#define BIT(x) (1 << x)

#define INFSYS_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

namespace InfSysCAD {

	template<typename T>
	using Scope = std::unique_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Ref = std::shared_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}
}

#include "CAD/Debug/Log.h"
#include "CAD/Debug/Assert.h"