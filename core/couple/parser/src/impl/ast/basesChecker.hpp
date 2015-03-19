#include <errorInfo.hpp>
#include "../ast.hpp"
#include <vector>
#include <map>
#include <algorithm>

namespace  dci { namespace couple { namespace parser { namespace impl { namespace ast
{
    class BasesChecker
        : public boost::static_visitor<bool>
    {
        std::vector<ErrorInfo> &_errs;

    public:
        BasesChecker(std::vector<ErrorInfo> &errs)
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
            res &= check(v.get());
            res &= walk(v.get());
            return res;
        }

    private:
        bool check(...)
        {
            return true;
        }

        template <class T>
        typename std::enable_if<sizeof(T::bases)!=0, bool>::type check(T *v)
        {
            if(!v->bases)
            {
                return true;
            }

            bool res = true;

            //duplicate direct bases
            auto direct = v->bases->instances;
            std::sort(direct.begin(), direct.end());

            auto dupIter = std::adjacent_find(direct.begin(), direct.end());
            if(direct.end() != dupIter)
            {
                std::size_t idx = std::find(v->bases->instances.begin(), v->bases->instances.end(), *dupIter) - v->bases->instances.begin();
                const ScopedName &scopedName = v->bases->scopedNames[idx];
                _errs.emplace_back(ErrorInfo {
                                      scopedName->pos.begin().file(),
                                      static_cast<int>(scopedName->pos.begin().line()),
                                      static_cast<int>(scopedName->pos.begin().column()),
                                      "duplicate base type: "+scopedName->toString()});
                res = false;
            }

            //cycles in all bases
            std::set<T*> allBases;
            res &= collectAllBases(v, v->bases->instances, allBases);

            return res;
        }

    private:
        template <class T>
        bool collectAllBases(T *v, const std::vector<T*> &bases, std::set<T*> &allBases)
        {
            if(allBases.end() != allBases.find(v))
            {
                _errs.emplace_back(ErrorInfo {
                                      v->name->pos.begin().file(),
                                      static_cast<int>(v->name->pos.begin().line()),
                                      static_cast<int>(v->name->pos.begin().column()),
                                      "cycle in inheritance: "+v->name->value});
                return false;
            }

            allBases.insert(bases.begin(), bases.end());

            for(auto base : bases)
            {
                if(base->bases && !collectAllBases(v, base->bases->instances, allBases))
                {
                    return false;
                }
            }

            return true;
        }

    private:
        bool walk(...)
        {
            return true;
        }

        bool walk(SScope *v)
        {
            return exec(v->decls);
        }

    };

}}}}}
