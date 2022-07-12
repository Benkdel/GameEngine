#pragma once

#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace Amba {

	class Log
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }

	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;

	};

	#define AB_TRACE(...)  ::Amba::Log::GetCoreLogger()->trace(__VA_ARGS__)
	#define AB_INFO(...)   ::Amba::Log::GetCoreLogger()->info(__VA_ARGS__)
	#define AB_WARN(...)   ::Amba::Log::GetCoreLogger()->warn(__VA_ARGS__)
	#define AB_ERROR(...)  ::Amba::Log::GetCoreLogger()->error(__VA_ARGS__)
	#define AB_FATAL(...)  ::Amba::Log::GetCoreLogger()->fatal(__VA_ARGS__)
}
