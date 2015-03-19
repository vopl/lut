#include "../grammar.hpp"

namespace dci { namespace couple { namespace parser { namespace impl
{
    void Grammar::mkName()
    {
        ////////////////////////////////////////////////////////////////////////////////
        name =
            (  toks.kwvoid
               | toks.kwbool
               | toks.kwstring
               | toks.kwint8
               | toks.kwint16
               | toks.kwint32
               | toks.kwint64
               | toks.kwuint8
               | toks.kwuint16
               | toks.kwuint32
               | toks.kwuint64
               | toks.kwreal32
               | toks.kwreal64

               | toks.kwlist
               | toks.kwset
               | toks.kwmap
               | toks.kwptr
               | toks.kwarray

               | toks.kwscope
               | toks.kwinclude
               | toks.kwalias
               | toks.kwstruct
               | toks.kwvariant
               | toks.kwiface
               | toks.kwenum
               | toks.kwin
               | toks.kwout
               | toks.kwnowait

               | toks.id
            )
                [qi::_val = phx::construct<Name>(phx::new_<SName>())]
                [phx::bind(&SName::value, deref(qi::_val)) = phx::construct<std::string>(phx::begin(qi::_1), phx::end(qi::_1))]
                [phx::bind(&SName::pos, deref(qi::_val)) = qi::_1];
    }

}}}}
