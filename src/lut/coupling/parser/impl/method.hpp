#pragma once

#include "lut/coupling/parser/impl/common.hpp"

#include <boost/spirit/repository/home/qi.hpp>

namespace lut { namespace coupling { namespace parser { namespace impl
{

    enum class MethodDirection
    {
        in,
        out
    };

    struct Method
    {
        MethodDirection             _direction;
        //TypePtr                     _resultType;
        bool                        _nowait;
        std::string                 _name;
        //std::vector<AttributePtr>   _params;
    };
    using MethodPtr = std::shared_ptr<Method>;



    struct MethodParser
        : qi::grammar<TokIterator, MethodPtr(), qi::locals<MethodDirection, MethodPtr> >
    {
        MethodParser()
            : MethodParser::base_type(start)
        {

            start =
                    // in/out
                    (
                        toks.kwin [qi::_a = MethodDirection::in ] ||
                        toks.kwout[qi::_a = MethodDirection::out]
                    ) >>

                    qi::eps[qi::_b = phx::construct<MethodPtr>(phx::new_<Method>())] >>

                    // nowait
                    (
                        toks.kwnowait[bind(&Method::_nowait, deref(qi::_b)) = true] ||
                        qi::eps
                    ) >>

//                    // result type
//                    (
//                        toks.word[phx::bind(&Method::_resultType, deref(_b)) = _1] ||
//                        error("method result type expected")) >>
//                    ) >>

                    // name
                    (
                        toks.word[phx::bind(&Method::_name, deref(qi::_b)) = phx::construct<std::string>(phx::begin(qi::_1), phx::end(qi::_1))] ||
                        error(+"method name expected")
                    ) >>

                    // begin arguments
                    (toks.ob || error(+"'(' expected")) >>

                    // arguments list
//                    attribute[push one attr] % toks.comma >>

                    // end arguments
                    (toks.cb || error(+"')' expected")) >>

                    // end definition
                    (toks.semi || error(+"';' expected")) >>

                    qi::eps[qi::_val = qi::_b];

            error = boost::spirit::repository::qi::iter_pos[ phx::throw_(phx::construct<Error>(qi::_1, qi::_r1))];
        }

        class Error
                : public std::runtime_error
        {
        public:
            Error(TokIterator pos, const std::string &msg)
                : std::runtime_error(msg)
            {

            }

        };

        qi::rule<TokIterator, void(std::string)> error;

        qi::rule<TokIterator, MethodPtr(), qi::locals<MethodDirection, MethodPtr> > start;
    };

    MethodParser method;

}}}}
