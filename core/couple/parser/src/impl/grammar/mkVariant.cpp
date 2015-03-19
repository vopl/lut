#include "../grammar.hpp"

namespace dci { namespace couple { namespace parser { namespace impl
{
    void Grammar::mkVariant()
    {
        ////////////////////////////////////////////////////////////////////////////////
        variant =
            toks.kwvariant[qi::_val = phx::construct<Variant>(phx::new_<SVariant>())] >>
            (
                name[phx::bind(&SVariant::name, deref(qi::_val)) = qi::_1] |
                error(+"variant name expected")
            ) >>
            -baseVariants[phx::bind(&SVariant::bases, deref(qi::_val)) = qi::_1] >>
            (toks.ocb | error(+"'{' expected")) >>
            *(
                //decls[phx::insert(phx::bind(&SVariant::decls, deref(qi::_val)), phx::end(phx::bind(&SVariant::decls, deref(qi::_val))), phx::begin(qi::_1), phx::end(qi::_1))] |

                decls[
                    phx::for_each(
                        qi::_1,
                        phx::lambda
                            (phx::local_names::_a = phx::bind(&SVariant::decls, deref(qi::_val)))
                            [phx::push_back(phx::local_names::_a, phx::arg_names::_1)]
                    )
                ] |

                variantField[phx::push_back(phx::bind(&SVariant::fields, deref(qi::_val)), qi::_1)]
            ) >>
            (toks.ccb | error(+"'}' expected"));
    }

}}}}
