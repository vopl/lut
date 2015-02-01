#include "lut/coupling/parser/impl/grammar.hpp"
#include "lut/coupling/parser/impl/grammarError.hpp"

namespace lut { namespace coupling { namespace parser { namespace impl
{

    Grammar::Grammar(ParseState &parseState)
        : Grammar::base_type(decls)
        , _parseState(parseState)
    {

        mkPrimitive();
        mkList();
        mkSet();
        mkMap();
        mkPtr();
        mkArray();
        mkTypeName();
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

        ////////////////////////////////////////////////////////////////////////////////
        error = boost::spirit::repository::qi::iter_pos[ phx::throw_(phx::construct<GrammarError>(qi::_1, qi::_r1))];
    }

}}}}
