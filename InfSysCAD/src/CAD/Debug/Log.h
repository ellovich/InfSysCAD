#pragma once

#include "CAD/Core/Base.h"

// This ignores all warnings raised inside External headers
#pragma warning(push, 0)
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"
#pragma warning(pop)

namespace InfSysCAD
{
	class Log
	{
	public:
		static void Init();
		static std::shared_ptr<spdlog::logger>& GetLogger() { return s_Logger; }
	
	private:
		static std::shared_ptr<spdlog::logger> s_Logger;
	};
}
						    
#define INFSYS_TRACE(...)	    ::InfSysCAD::Log::GetLogger()->trace(__VA_ARGS__)
#define INFSYS_INFO(...)	    ::InfSysCAD::Log::GetLogger()->info(__VA_ARGS__)
#define INFSYS_WARN(...)	    ::InfSysCAD::Log::GetLogger()->warn(__VA_ARGS__)
#define INFSYS_ERROR(...)	    ::InfSysCAD::Log::GetLogger()->error(__VA_ARGS__)
#define INFSYS_FATAL(...)	    ::InfSysCAD::Log::GetLogger()->fatal(__VA_ARGS__)