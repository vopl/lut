#include "lut/async/stable.hpp"
#include "lut/mm/impl/stack.hpp"

namespace lut { namespace mm { namespace impl
{
    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    Stack::Stack()
    {
        assert(0);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    Stack::~Stack()
    {
        assert(0);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    void Stack::compact()
    {
        return Layout::protectTo((char *)alloca(1) + Config::_stackKeepProtectedBytes);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    const lut::mm::Stack *Stack::impl2Face(Stack *impl)
    {
        return Layout::impl2Face(static_cast<Layout *>(impl));
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    Stack *Stack::face2Impl(const lut::mm::Stack *face)
    {
        return static_cast<Stack *>(Layout::face2Impl(face));
    }

}}}
