#include "lut/async/stable.hpp"
#include "lut/async/impl/mm/allocatorGeneral.hpp"


namespace lut { namespace async { namespace impl { namespace mm
{
    AllocatorGeneral::AllocatorGeneral()
    {
        assert(0);
    }

    AllocatorGeneral &AllocatorGeneral::instance()
    {
        assert(0);
    }

    AllocatorGeneral::~AllocatorGeneral()
    {
        assert(0);
    }

    bool AllocatorGeneral::threadInit()
    {
        assert(0);
    }

    bool AllocatorGeneral::threadDeinit()
    {
        assert(0);
    }

    const Stack *AllocatorGeneral::stackAlloc()
    {
        assert(0);
    }

    bool AllocatorGeneral::stackFree(const Stack *stack)
    {
        assert(0);
    }

    bool AllocatorGeneral::stackCompact(const Stack *stack)
    {
        assert(0);
    }

    bool AllocatorGeneral::vmAccessHandler(void *addr)
    {
        assert(0);
    }

    bool AllocatorGeneral::s_vmAccessHandler(void *addr)
    {
        assert(0);
    }

    AllocatorGeneral AllocatorGeneral::_instance;
}}}}


