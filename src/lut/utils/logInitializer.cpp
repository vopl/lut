#include "lut/utils/logInitializer.hpp"

#include <log4cplus/layout.h>
#include <log4cplus/helpers/pointer.h>
#include <log4cplus/consoleappender.h>
#include <log4cplus/fileappender.h>

namespace lut { namespace utils
{
	namespace po = boost::program_options;
	using namespace log4cplus;



	//////////////////////////////////////////////////////////////////////////
	void LogInitializer::fillOptions(Options &options, int default_)
	{
		options.addOption(
			"enable",
			po::value<bool>()->default_value(true),
			"enable this log");

		std::string name = options.prefix();
		if(name.find(".log") == name.size()-4)
		{
			name = name.substr(0, name.size()-4);
		}

		options.addOption(
			"name",
			po::value<std::string>()->default_value(name),
			"log hierarhical name");

		options.addOption(
			"level",
			po::value<std::string>()->default_value("trace"),
			"level, one of [trace, debug, info, warn, error, fatal, off]");

		options.addOption(
			"additivity",
			po::value<bool>()->default_value(false),
			"hierarhical additivity flag");


		//файл уровень, паттерн, ротатор
		options.addOption(
			"file",
			po::value<bool>()->default_value((default_ & _defaultFile)?true:false),
			"use file appender");

		options.addOption(
			"file.dir",
			po::value<std::string>()->default_value("../log"),
			"directory for log files");

		options.addOption(
			"file.pattern",
			po::value<std::string>()->default_value("%d{%H:%M:%S.%q} %p: %m%n"),
			"line pattern");

		options.addOption(
			"file.rotator.period",
			po::value<std::string>()->default_value("daily"),
			"file rotator period (monthly, weekly, daily, twice_daily, hourly, minutely)");

		options.addOption(
			"file.rotator.amount",
			po::value<int>()->default_value(150),
			"file rotator history amount");

		//консоль уровень, паттерн
		options.addOption(
			"console",
			po::value<bool>()->default_value((default_ & _defaultConsole)?true:false),
			"use console appender");

		options.addOption(
			"console.pattern",
			po::value<std::string>()->default_value("%d{%H:%M:%S.%q} %-5p %c: %m%n"),
			"line pattern");
	}

	//////////////////////////////////////////////////////////////////////////
	void LogInitializer::init(log4cplus::Logger &instance, Options &options)
	{
		instance = Logger::getInstance(options["name"].as<std::string>());

		if(options.count("enable") && options["enable"].as<bool>())
		{
			instance.setLogLevel(level(options["level"].as<std::string>()));
			instance.setAdditivity(options["additivity"].as<bool>());

			if(options.count("file") && options["file"].as<bool>())
			{
				SharedAppenderPtr fileApp(new DailyRollingFileAppender(
						options["file.dir"].as<std::string>()+"/"+options.prefix(),
						rperiod(options["file.rotator.period"].as<std::string>()),
						true,
						options["file.rotator.amount"].as<int>()));
				fileApp->setName("file");
				fileApp->setLayout(std::auto_ptr<Layout>(new PatternLayout(
						options["file.pattern"].as<std::string>())));

				instance.addAppender(fileApp);
			}

			if(options.count("console") && options["console"].as<bool>())
			{
				SharedAppenderPtr conApp(new ConsoleAppender());
				conApp->setName("STDOUT");
				conApp->setLayout(std::auto_ptr<Layout>(new PatternLayout(
						options["console.pattern"].as<std::string>())));
				instance.addAppender(conApp);
			}
		}
	}

	//////////////////////////////////////////////////////////////////////////
	DailyRollingFileSchedule LogInitializer::rperiod(const std::string &value)
	{
		typedef std::map<std::string, DailyRollingFileSchedule> TMap;
        static TMap m {
                std::make_pair("monthly", MONTHLY),
                std::make_pair("weekly", WEEKLY),
                std::make_pair("daily", DAILY),
                std::make_pair("twice_daily", TWICE_DAILY),
                std::make_pair("hourly", HOURLY),
                std::make_pair("minutely", MINUTELY),
        };

		TMap::iterator iter = m.find(value);
		if(m.end() != iter)
		{
			return iter->second;
		}

		return DAILY;
	}

	//////////////////////////////////////////////////////////////////////////
	LogLevel LogInitializer::level(const std::string &value)
	{
		typedef std::map<std::string, LogLevel> TMap;
        static TMap m {
                std::make_pair("off", OFF_LOG_LEVEL),
                std::make_pair("fatal", FATAL_LOG_LEVEL),
                std::make_pair("error", ERROR_LOG_LEVEL),
                std::make_pair("warn", WARN_LOG_LEVEL),
                std::make_pair("info", INFO_LOG_LEVEL),
                std::make_pair("debug", DEBUG_LOG_LEVEL),
                std::make_pair("trace", TRACE_LOG_LEVEL),
        };

		TMap::iterator iter = m.find(value);
		if(m.end() != iter)
		{
			return iter->second;
		}

		return TRACE_LOG_LEVEL;
	}

}}
