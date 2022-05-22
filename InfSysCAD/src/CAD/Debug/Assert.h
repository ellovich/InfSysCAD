#pragma once
#include "CAD/Debug/Base.h"
#include "CAD/Debug/Log.h"
#include <filesystem>

#ifdef INFSYS_ENABLE_ASSERTS

// Alteratively we could use the same "default" message for both "WITH_MSG" and "NO_MSG" and
// provide support for custom formatting by concatenating the formatting string instead of having the format inside the default message
#define INFSYS_INTERNAL_ASSERT_IMPL(type, check, msg, ...) { if(!(check)) { INFSYS##type##ERROR(msg, __VA_ARGS__); INFSYS_DEBUGBREAK(); } }
#define INFSYS_INTERNAL_ASSERT_WITH_MSG(type, check, ...) INFSYS_INTERNAL_ASSERT_IMPL(type, check, "Assertion failed: {0}", __VA_ARGS__)
#define INFSYS_INTERNAL_ASSERT_NO_MSG(type, check) INFSYS_INTERNAL_ASSERT_IMPL(type, check, "Assertion '{0}' failed at {1}:{2}", INFSYS_STRINGIFY_MACRO(check), std::filesystem::path(__FILE__).filename().string(), __LINE__)

#define INFSYS_INTERNAL_ASSERT_GET_MACRO_NAME(arg1, arg2, macro, ...) macro
#define INFSYS_INTERNAL_ASSERT_GET_MACRO(...) INFSYS_EXPAND_MACRO( INFSYS_INTERNAL_ASSERT_GET_MACRO_NAME(__VA_ARGS__, INFSYS_INTERNAL_ASSERT_WITH_MSG, INFSYS_INTERNAL_ASSERT_NO_MSG) )

// Currently accepts at least the condition and one additional parameter (the message) being optional
#define INFSYS_ASSERT(...) INFSYS_EXPAND_MACRO( INFSYS_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_, __VA_ARGS__) )
#define INFSYS_CORE_ASSERT(...) INFSYS_EXPAND_MACRO( INFSYS_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_CORE_, __VA_ARGS__) )
#else
#define INFSYS_ASSERT(...)
#define INFSYS_CORE_ASSERT(...)
#endif