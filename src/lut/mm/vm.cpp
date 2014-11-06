#include "lut/stable.hpp"
#include "lut/mm/vm.hpp"
#include "lut/mm/system.hpp"

/*
 * системное ограничение на количество сегментов vm для процесса,
 * /proc/sys/vm/max_map_count,
 * sysctl vm.max_map_count=16777216
 *
 * по этому лимиту будет отваливаться mprotect с ошибкой ENOMEM
 *
 *
*/

namespace lut { namespace mm { namespace vm
{
    namespace
    {
        ////////////////////////////////////////////////////////////////////////////////
        extern char stateArea[];

        struct State
        {
            TVmAccessHandler _accessHandler;

            size_t _altStackSize;
            char _altStackArea[SIGSTKSZ];
            struct sigaltstack _oldAltStack;
            struct sigaction _oldAction;

            State()
                : _accessHandler()
                , _altStackSize(SIGSTKSZ)
                , _altStackArea()
                , _oldAction()
            {
            }

            ~State()
            {
            }

            void * operator new(size_t size)
            {
                (void)size;
                return &stateArea;
            }

            void operator delete(void *ptr)
            {
                (void)ptr;
            }
        };
        char stateArea[sizeof(State)];

        State *_state = nullptr;

        ////////////////////////////////////////////////////////////////////////////////
        void segvHandler(int signal_number, siginfo_t *info, void *ptr)
        {
            State *state = _state;

            if(state)
            {
                if(state->_accessHandler(info->si_addr))
                {
                    return;
                }

                fprintf(stderr, "call SIGSEGV default handler for %p\n", info->si_addr);
                if(state->_oldAction.sa_flags & SA_SIGINFO)
                {
                    return state->_oldAction.sa_sigaction(signal_number, info, ptr);
                }
                else
                {
                    return state->_oldAction.sa_handler(signal_number);
                }
            }

            fprintf(stderr, "unable to handle SIGSEGV\n");
            std::abort();
            return;
        }
    }


    ////////////////////////////////////////////////////////////////////////////////
    bool init(TVmAccessHandler accessHandler)
    {
        if(_state)
        {
            fprintf(stderr, "vm::threadInit: secondary call\n");
            return false;
        }
        _state = new State;
        _state->_accessHandler = accessHandler;

        struct sigaltstack altstack;
        memset(&altstack, 0, sizeof(altstack));
        altstack.ss_size = _state->_altStackSize;
        altstack.ss_sp = _state->_altStackArea;
        altstack.ss_flags = 0;
        if(sigaltstack(&altstack, &_state->_oldAltStack))
        {
            delete _state;
            _state = nullptr;
            perror("sigaltstack");
            return false;
        }

        struct sigaction sa;
        memset(&sa, 0, sizeof(sa));
        sa.sa_sigaction = &segvHandler;
        sa.sa_flags = SA_SIGINFO | SA_ONSTACK;
        sigfillset (&sa.sa_mask);
        if(sigaction(SIGSEGV, &sa, &_state->_oldAction))
        {
            delete _state;
            _state = nullptr;
            perror("sigaction");
            return false;
        }

        return true;

    }

    ////////////////////////////////////////////////////////////////////////////////
    bool deinit(TVmAccessHandler accessHandler)
    {
        if(!_state)
        {
            fprintf(stderr, "vm::threadDeinit: already deinited\n");
            return false;
        }

        if(_state->_accessHandler != accessHandler)
        {
            fprintf(stderr, "vm::threadDeinit: urong accessHandler\n");
            return false;
        }

        if(sigaction(SIGSEGV, &_state->_oldAction, NULL))
        {
            perror("sigaction");
        }

        if(sigaltstack(&_state->_oldAltStack, NULL))
        {
            perror("sigaltstack");
        }

        delete _state;
        _state = nullptr;
        return true;
    }

    ////////////////////////////////////////////////////////////////////////////////
    void *alloc(std::size_t size)
    {
        void *addr = mmap(
                            NULL,
                            size,
                            PROT_NONE,
                            MAP_ANONYMOUS|MAP_PRIVATE,
                            0,
                            0);

        if(MAP_FAILED == addr)
        {
            perror("mmap");
            return 0;
        }

        return addr;
    }

    ////////////////////////////////////////////////////////////////////////////////
    bool free(void *addr, std::size_t size)
    {
        if(munmap(addr, size))
        {
            perror("munmap");
            return false;
        }

        return true;
    }

    ////////////////////////////////////////////////////////////////////////////////
    bool protect(void *addr, std::size_t size, bool access)
    {
        if(mprotect(addr, size, access ? (PROT_READ|PROT_WRITE) : PROT_NONE))
        {
            perror("mprotect");
            return false;
        }

        return true;
    }

}}}
