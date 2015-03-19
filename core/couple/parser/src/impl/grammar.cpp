#include "grammar.hpp"
#include "grammarError.hpp"

namespace dci { namespace couple { namespace parser { namespace impl
{

    Grammar::Grammar(ParseState &parseState)
        : Grammar::base_type(file)
        , _parseState(parseState)
    {

        mkPrimitive();
        mkList();
        mkSet();
        mkMap();
        mkPtr();
        mkArray();
        mkName();
        mkScopedName();
        mkTypeUse();
        mkAlias();
        mkBases();
        mkVariantField();
        mkVariant();
        mkStructField();
        mkStruct();
        mkEnumField();
        mkEnum();
        mkMethodParam();
        mkMethod();
        mkIface();
        mkScope();
        mkInclude();
        mkDecl();
        mkDecls();
        mkFile();

        ////////////////////////////////////////////////////////////////////////////////
        error = boost::spirit::repository::qi::iter_pos[ phx::throw_(phx::construct<GrammarError>(qi::_1, qi::_r1))];
    }

}}}}
