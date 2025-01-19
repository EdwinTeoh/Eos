/**********************************************************************************
* \file			Logger.h

* \brief		A wrapper to handle spdlog
*

* \author		Jeroen Tan Yi Xu

* \copyright	Copyright (c) 2021 DigiPen Institute of Technology. Reproduction
*				or disclosure of this file or its contents without the prior
*				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#ifndef LOGGER_H_
#define LOGGER_H_

#pragma warning(push)
#pragma warning(disable : 4081)
#pragma warning(disable : 4706)
#include "spdlog\spdlog.h"
#include "spdlog\sinks\stdout_color_sinks.h"
#include "spdlog\sinks\basic_file_sink.h"
#pragma warning(pop)
#include <memory>
#include "Tools\Singleton.h"

namespace Eos
{
	class Logger : public Singleton<Logger>
	{
	public:
		static void Init();
		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return m_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return m_ClientLogger; }

		static std::shared_ptr<spdlog::logger> m_CoreLogger;
		static std::shared_ptr<spdlog::logger> m_ClientLogger;
	};
}

#define PE_CORE_TRACE(...)					Eos::Logger::GetCoreLogger()->trace(__VA_ARGS__);
#define PE_CORE_INFO(...)					Eos::Logger::GetCoreLogger()->info(__VA_ARGS__);
#define PE_CORE_DEBUG(...)					Eos::Logger::GetCoreLogger()->debug(__VA_ARGS__);
#define PE_CORE_WARN(...)					Eos::Logger::GetCoreLogger()->warn(__VA_ARGS__);
#define PE_CORE_ERROR(...)					Eos::Logger::GetCoreLogger()->error(__VA_ARGS__);
#define PE_CORE_CRITICAL(...)				Eos::Logger::GetCoreLogger()->critical(__VA_ARGS__);

#define PE_TRACE(...)						Eos::Logger::GetClientLogger()->trace(__VA_ARGS__);
#define PE_INFO(...)						Eos::Logger::GetClientLogger()->info(__VA_ARGS__);
#define PE_DEBUG(...)						Eos::Logger::GetClientLogger()->debug(__VA_ARGS__);
#define PE_WARN(...)						Eos::Logger::GetClientLogger()->warn(__VA_ARGS__);
#define PE_ERROR(...)						Eos::Logger::GetClientLogger()->error(__VA_ARGS__);
#define PE_CRITICAL(...)					Eos::Logger::GetClientLogger()->critical(__VA_ARGS__);


#endif 
