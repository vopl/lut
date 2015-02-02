#include "lut/coupling/parser/impl/grammar.hpp"

namespace lut { namespace coupling { namespace parser { namespace impl
{
    void Grammar::mkBases()
    {
        ////////////////////////////////////////////////////////////////////////////////
        baseStructs =
            toks.colon[qi::_val = phx::construct<BaseStructs>(phx::new_<SBaseStructs>())] >>
            (
                (
                    typeName[phx::push_back(phx::bind(&SBaseStructs::names, deref(qi::_val)), qi::_1)] |
                    error(+"base struct name expected")
                ) % toks.comma
            );

        ////////////////////////////////////////////////////////////////////////////////
        baseVariants =
            toks.colon[qi::_val = phx::construct<BaseVariants>(phx::new_<SBaseVariants>())] >>
            (
                (
                    typeName[phx::push_back(phx::bind(&SBaseVariants::names, deref(qi::_val)), qi::_1)] |
                    error(+"base variant name expected")
                ) % toks.comma
            );

        ////////////////////////////////////////////////////////////////////////////////
        baseEnums =
            toks.colon[qi::_val = phx::construct<BaseEnums>(phx::new_<SBaseEnums>())] >>
            (
                (
                    typeName[phx::push_back(phx::bind(&SBaseEnums::names, deref(qi::_val)), qi::_1)] |
                    error(+"base enum type expected")
                ) % toks.comma
            );

        ////////////////////////////////////////////////////////////////////////////////
        baseIfaces =
            toks.colon[qi::_val = phx::construct<BaseIfaces>(phx::new_<SBaseIfaces>())] >>
            (
                (
                    typeName[phx::push_back(phx::bind(&SBaseIfaces::names, deref(qi::_val)), qi::_1)] |
                    error(+"base iface type expected")
                ) % toks.comma
            );
    }

}}}}
