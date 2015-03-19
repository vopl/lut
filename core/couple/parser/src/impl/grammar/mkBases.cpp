#include "../grammar.hpp"

namespace dci { namespace couple { namespace parser { namespace impl
{
    void Grammar::mkBases()
    {
        ////////////////////////////////////////////////////////////////////////////////
        baseStructs =
            toks.colon[qi::_val = phx::construct<BaseStructs>(phx::new_<SBaseStructs>())] >>
            (
                (
                    scopedName[phx::push_back(phx::bind(&SBaseStructs::scopedNames, deref(qi::_val)), qi::_1)] |
                    error(+"base struct name expected")
                ) % toks.comma
            );

        ////////////////////////////////////////////////////////////////////////////////
        baseVariants =
            toks.colon[qi::_val = phx::construct<BaseVariants>(phx::new_<SBaseVariants>())] >>
            (
                (
                    scopedName[phx::push_back(phx::bind(&SBaseVariants::scopedNames, deref(qi::_val)), qi::_1)] |
                    error(+"base variant name expected")
                ) % toks.comma
            );

        ////////////////////////////////////////////////////////////////////////////////
        baseEnums =
            toks.colon[qi::_val = phx::construct<BaseEnums>(phx::new_<SBaseEnums>())] >>
            (
                (
                    scopedName[phx::push_back(phx::bind(&SBaseEnums::scopedNames, deref(qi::_val)), qi::_1)] |
                    error(+"base enum type expected")
                ) % toks.comma
            );

        ////////////////////////////////////////////////////////////////////////////////
        baseIfaces =
            toks.colon[qi::_val = phx::construct<BaseIfaces>(phx::new_<SBaseIfaces>())] >>
            (
                (
                    scopedName[phx::push_back(phx::bind(&SBaseIfaces::scopedNames, deref(qi::_val)), qi::_1)] |
                    error(+"base iface type expected")
                ) % toks.comma
            );
    }

}}}}
