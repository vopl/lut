#include "../grammar.hpp"

namespace dci { namespace couple { namespace parser { namespace impl
{
    void Grammar::mkStruct()
    {
        ////////////////////////////////////////////////////////////////////////////////
        struct_ =
            toks.kwstruct[qi::_val = phx::construct<Struct>(phx::new_<SStruct>())] >>
            (
                name[phx::bind(&SStruct::name, deref(qi::_val)) = qi::_1] |
                error(+"structure name expected")
            ) >>
            -baseStructs[phx::bind(&SStruct::bases, deref(qi::_val)) = qi::_1]>>
            (toks.ocb | error(+"'{' expected")) >>
            *(
                //decls[phx::insert(phx::bind(&SStruct::decls, deref(qi::_val)), phx::end(phx::bind(&SStruct::decls, deref(qi::_val))), phx::begin(qi::_1), phx::end(qi::_1))] |

                decls[
                    phx::for_each(
                        qi::_1,
                        phx::lambda
                            (phx::local_names::_a = phx::bind(&SStruct::decls, deref(qi::_val)))
                            [phx::push_back(phx::local_names::_a, phx::arg_names::_1)]
                    )
                ] |

                structField[phx::push_back(phx::bind(&SStruct::fields, deref(qi::_val)), qi::_1)]
            ) >>
            (toks.ccb | error(+"'}' expected"));
    }

}}}}
