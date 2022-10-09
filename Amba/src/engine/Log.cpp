#include "Log.h"

namespace Amba {

	std::shared_ptr<spdlog::logger> Log::s_CoreLogger;

	void Log::Init()
	{
		spdlog::set_pattern("%^[%T] %n: %v%$");
		s_CoreLogger = spdlog::stdout_color_mt("AMBA"); // review this with spdlog wiki
		s_CoreLogger->set_level(spdlog::level::trace);
	}
}