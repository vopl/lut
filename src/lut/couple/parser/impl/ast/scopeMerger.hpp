#include "lut/couple/parser/errorInfo.hpp"
#include "lut/couple/parser/impl/ast.hpp"
#include <algorithm>
#include <map>

namespace  lut { namespace couple { namespace parser { namespace impl { namespace ast
{
    class ScopeMerger
        : public boost::static_visitor<bool>
    {
        std::map<std::string, Scope> _scopes;
    public:
        void exec(const Scope &s)
        {
            return exec(s->decls);
        }

    private:
        void exec(std::vector<Decl> &decls)
        {
            decls.erase(
                std::remove_if(
                    decls.begin(),
                    decls.end(),
                    [&](Decl &d)->bool{return boost::apply_visitor(*this, d);}
                )
                , decls.end()
            );
        }

    public:
        bool operator()(Scope &v)
        {
            auto ires = _scopes.insert(std::make_pair(v->name->value, v));
            if(ires.second)
            {
                return false;
            }

            Scope old = ires.first->second;
            old->decls.insert(old->decls.end(), v->decls.begin(), v->decls.end());

            ScopeMerger().exec(old->decls);

            return true;
        }

        bool operator()(const Alias &v)
        {
            return false;
        }
        bool operator()(const Enum &v)
        {
            return false;
        }

        template <class TScope>
        bool operator()(const TScope &v)
        {
            ScopeMerger().exec(v->decls);
            return false;
        }
    };
}}}}}
