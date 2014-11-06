#pragma once

#include "lut/utils/options.hpp"
#include <log4cplus/logger.h>
#include <log4cplus/fileappender.h>

namespace lut { namespace utils
{
	class LogInitializer
	{
	public:
		static void fillOptions(Options &options, int default_=_defaultNull);
		static void init(log4cplus::Logger &instance, Options &options);

		static const int _defaultNull = 0x0;
		static const int _defaultFile = 0x1;
		static const int _defaultConsole = 0x2;

	private:
		static log4cplus::DailyRollingFileSchedule rperiod(const std::string &value);
		static log4cplus::LogLevel level(const std::string &value);

	};
}}
