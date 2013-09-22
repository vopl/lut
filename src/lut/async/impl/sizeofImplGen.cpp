#include "lut/async/stable.hpp"
#include "lut/async/impl/scheduler.hpp"
#include "lut/async/impl/event.hpp"
#include "lut/async/impl/mutex.hpp"

#include <iostream>
using namespace std;

int main()
{
    cout<<

           "#ifndef _ASYNC_SIZEOFIMPL_HPP_\n"
           "#define _ASYNC_SIZEOFIMPL_HPP_\n"
           "\n"
           "#include <cstddef>\n"
           "\n"
           "namespace lut { namespace async\n"
           "{\n"

           "    template <class T>\n"
           "    struct sizeofImpl\n"
           "    {\n"
           "    };\n"
           "\n"


#define ONE(name) \
           "    namespace impl { class " #name "; } \n" \
           "    template <> struct sizeofImpl<impl::" #name "> { static const std::size_t _value =  " << sizeof(lut::async::impl::name) << ";};\n\n"
           ONE(Scheduler)
           ONE(Event)
           ONE(Mutex)


           "}}\n"
           "\n"
           "#endif\n"
;

    return 0;
}