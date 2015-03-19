#include <errorInfo.hpp>
#include "../ast.hpp"
#include <vector>
#include <map>
#include <algorithm>

namespace  dci { namespace couple { namespace parser { namespace impl { namespace ast
{
    class NamesChecker
        : public boost::static_visitor<bool>
    {
        std::vector<ErrorInfo> &_errs;

        std::map<std::string, Name> _typeNames;

    public:
        NamesChecker(std::vector<ErrorInfo> &errs)
            : _errs(errs)
        {}

    public:
        bool exec(const Scope &s)
        {
            return exec(s->decls);
        }

    private:
        bool exec(const std::vector<Decl> &decls)
        {
            return std::accumulate(
                decls.begin(),
                decls.end(),
                true,
                [&](bool v, const Decl &d)->bool{return boost::apply_visitor(*this, d) && v;}
            );
        }

    public:
        template <class T>
        bool operator()(const T &v)
        {
            bool res = true;
            res &= checkTypeName(v.get());
            res &= checkFields(v.get());
            res &= checkChildren(v.get());
            return res;
        }

    private:
        bool checkTypeName(...)
        {
            return true;
        }

        template <class T>
        typename std::enable_if<sizeof(T::name)!=0, bool>::type checkTypeName(const T *v)
        {
            const Name &cur = v->name;
            auto ires = _typeNames.insert(std::make_pair(cur->value, cur));
            if(!ires.second)
            {
                _errs.emplace_back(ErrorInfo {
                                      cur->pos.begin().file(),
                                      static_cast<int>(cur->pos.begin().line()),
                                      static_cast<int>(cur->pos.begin().column()),
                                      "duplicate name: "+cur->value});

                const Name &prev = ires.first->second;

                _errs.emplace_back(ErrorInfo {
                                      prev->pos.begin().file(),
                                      static_cast<int>(prev->pos.begin().line()),
                                      static_cast<int>(prev->pos.begin().column()),
                                      "previous declaration of "+prev->value});

                return false;
            }

            return true;
        }

    private:
        bool checkFields(...)
        {
            return true;
        }

        template <class T>
        typename std::enable_if<sizeof(T::fields)!=0, bool>::type checkFields(const T *v)
        {
            std::map<std::string, Name> fieldNames;

            bool res = true;
            for(const auto &f : v->fields)
            {
                const Name &cur = f->name;

                if(cur)
                {
                    auto ires = fieldNames.insert(std::make_pair(cur->value, cur));
                    if(!ires.second)
                    {
                        _errs.emplace_back(ErrorInfo {
                                              cur->pos.begin().file(),
                                              static_cast<int>(cur->pos.begin().line()),
                                              static_cast<int>(cur->pos.begin().column()),
                                              "duplicate name: "+cur->value});

                        const Name &prev = ires.first->second;

                        _errs.emplace_back(ErrorInfo {
                                              prev->pos.begin().file(),
                                              static_cast<int>(prev->pos.begin().line()),
                                              static_cast<int>(prev->pos.begin().column()),
                                              "previous declaration of "+prev->value});
                        res = false;
                    }
                }
            }

            return res;
        }

    private:
        bool checkChildren(...)
        {
            return true;
        }

        template <class T>
        typename std::enable_if<std::is_base_of<SScope, T>::value, bool>::type checkChildren(const T *v)
        {
            return NamesChecker(_errs).exec(v->decls);
        }
    };

}}}}}
