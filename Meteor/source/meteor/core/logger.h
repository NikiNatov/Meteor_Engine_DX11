#pragma once

#include <spdlog\spdlog.h>

namespace meteor
{
	class Logger
	{
	public:
		Logger(const Logger&) = delete;
		Logger& operator=(const Logger&) = delete;

		static void								Initialize();

		static std::shared_ptr<spdlog::logger>  GetEngineLogger() { return ms_EngineLogger; }
		static std::shared_ptr<spdlog::logger>  GetAppLogger() { return ms_AppLogger; }
	private:
		Logger();
	private:
		static std::shared_ptr<spdlog::logger> ms_EngineLogger;
		static std::shared_ptr<spdlog::logger> ms_AppLogger;
	};

}
	#define METEOR_ENGINE_LOG_TRACE(...)		meteor::Logger::GetEngineLogger()->trace(__VA_ARGS__)
	#define METEOR_ENGINE_LOG_INFO(...)			meteor::Logger::GetEngineLogger()->info(__VA_ARGS__)
	#define METEOR_ENGINE_LOG_WARNING(...)		meteor::Logger::GetEngineLogger()->warn(__VA_ARGS__)
	#define METEOR_ENGINE_LOG_ERROR(...)		meteor::Logger::GetEngineLogger()->error(__VA_ARGS__)
	#define METEOR_ENGINE_LOG_CRITICAL(...)		meteor::Logger::GetEngineLogger()->critical(__VA_ARGS__)

	#define METEOR_LOG_TRACE(...)				meteor::Logger::GetAppLogger()->trace(__VA_ARGS__)
	#define METEOR_LOG_INFO(...)				meteor::Logger::GetAppLogger()->info(__VA_ARGS__)
	#define METEOR_LOG_WARNING(...)				meteor::Logger::GetAppLogger()->warn(__VA_ARGS__)
	#define METEOR_LOG_ERROR(...)				meteor::Logger::GetAppLogger()->error(__VA_ARGS__)
	#define METEOR_LOG_CRITICAL(...)			meteor::Logger::GetAppLogger()->critical(__VA_ARGS__)