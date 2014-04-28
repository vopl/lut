#include "lut/utils/options.hpp"

#include <fstream>

namespace lut { namespace utils
{
	using namespace boost::program_options;

	//////////////////////////////////////////////////////////////////
	Options::Options(const char *prefix)
        : _prefix(prefix?prefix:"")
        , _vars()
        , _desc(_prefix)
	{
	}

	//////////////////////////////////////////////////////////////////
	Options::~Options()
	{
	}

	//////////////////////////////////////////////////////////////////
	void Options::addOption(const char *name, const char *descr)
	{
		_desc.add_options() (wrap(name).c_str(), descr);
	}

	//////////////////////////////////////////////////////////////////
	void Options::addOption(const char *name, const boost::program_options::value_semantic *sem)
	{
		_desc.add_options() (wrap(name).c_str(), sem);
	}

	//////////////////////////////////////////////////////////////////
	void Options::addOption(const char *name, const boost::program_options::value_semantic *sem, const char *descr)
	{
		_desc.add_options() (wrap(name).c_str(), sem, descr);
	}

	//////////////////////////////////////////////////////////////////////////
	void Options::addOptions(Options &other)
	{
		_desc.add(other.desc());
	}


	//////////////////////////////////////////////////////////////////
	void Options::store(boost::program_options::parsed_options *po1, boost::program_options::parsed_options *po2)
	{
		if(po1)
		{
			boost::program_options::store(*po1, _vars);
		}

		if(po2)
		{
			boost::program_options::store(*po2, _vars);
		}
		boost::program_options::notify(_vars);
	}

	//////////////////////////////////////////////////////////////////
	const std::string &Options::prefix()
	{
		return _prefix;
	}

	//////////////////////////////////////////////////////////////////
	size_t Options::count(const std::string &name) const
	{
		return _vars.count(wrap(name));
	}

	//////////////////////////////////////////////////////////////////
	const boost::program_options::variable_value & Options::operator[](const std::string &name) const
	{
		return _vars[wrap(name)];
	}

	//////////////////////////////////////////////////////////////////
	options_description &Options::desc()
	{
		return _desc;
	}

	//////////////////////////////////////////////////////////////////
	variables_map &Options::vars()
	{
		return _vars;
	}

	//////////////////////////////////////////////////////////////////
	const options_description &Options::desc() const
	{
		return _desc;
	}

	//////////////////////////////////////////////////////////////////
	const variables_map &Options::vars() const
	{
		return _vars;
	}

	//////////////////////////////////////////////////////////////////
	std::string Options::wrap(const char *name) const
	{
		if(_prefix.empty())
		{
			return name;
		}

		if(!name || !name[0])
		{
			return _prefix;
		}

		return _prefix + "." + name;
	}

	//////////////////////////////////////////////////////////////////
	std::string Options::wrap(const std::string &name) const
	{
        if(_prefix.empty())
        {
            return name;
        }

        if(name.empty())
        {
            return _prefix;
        }

        return _prefix + "." + name;
    }

}}
