#include "lut/async/stable.hpp"

#define GENERATE_SIZEOFIMPL 1

#include "lut/async/impl/task.hpp"

#include "lut/async/impl/mutex.hpp"
#include "lut/async/impl/recursiveMutex.hpp"
#include "lut/async/impl/semaphore.hpp"
#include "lut/async/impl/event.hpp"
#include "lut/async/impl/conditionVariable.hpp"

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

           ONE(Task)

           ONE(Mutex)
           ONE(RecursiveMutex)
           ONE(Semaphore)
           ONE(Event)
           ONE(ConditionVariable)

           "}}\n"
           "\n"
           "#endif\n"
;

    return 0;
}
