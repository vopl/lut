#ifndef _LUT_UTILS_OPTIONS_HPP_
#define _LUT_UTILS_OPTIONS_HPP_

#include <boost/program_options.hpp>
#include <memory>

namespace lut { namespace utils
{
	class Options;
    typedef std::shared_ptr<Options> OptionsPtr;

	class Options
	{
	public:
		Options(const char *prefix);
		virtual ~Options();

	public:
		void addOption(const char *name, const char *descr);
		void addOption(const char *name, const boost::program_options::value_semantic *sem);
		void addOption(const char *name, const boost::program_options::value_semantic *sem, const char *descr);
		void addOptions(Options &other);

		void store(boost::program_options::parsed_options *po1, boost::program_options::parsed_options *po2=NULL);

		const std::string &prefix();
		size_t count(const std::string &name) const;
		const boost::program_options::variable_value & operator[](const std::string &name) const;

		boost::program_options::options_description & desc();
		boost::program_options::variables_map &vars();

		const boost::program_options::options_description & desc() const;
		const boost::program_options::variables_map &vars() const;

	private:
		std::string _prefix;
		boost::program_options::variables_map			_vars;
		boost::program_options::options_description		_desc;

	private:
		std::string wrap(const char *name) const;
		std::string wrap(const std::string &name) const;
	};
}}

#endif
