#include "lut/async/impl/sm/vm.hpp"

#include <unistd.h>
#include <sys/mman.h>
#include <signal.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/*
 * системное ограничение на количество сегментов vm для процесса,
 * /proc/sys/vm/max_map_count,
 * sysctl vm.max_map_count=16777216
 *
 * по этому лимиту будет отваливаться mprotect с ошибкой ENOMEM
 *
 *
*/

namespace lut { namespace async { namespace impl { namespace sm { namespace vm
{
    namespace
    {
        struct PerThreadState
        {
            TVmAccessHandler _accessHandler;

            std::size_t _altStackSize;
            void *_altStackArea;
            struct sigaltstack _oldAltStack;
            struct sigaction _oldAction;

            PerThreadState(std::size_t altStackSize)
                : _accessHandler()
                , _altStackSize(altStackSize)
                , _altStackArea(malloc(altStackSize))
                , _oldAction()
            {
            }

            ~PerThreadState()
            {
                if(_altStackArea)
                {
                    ::free(_altStackArea);
                    _altStackArea = NULL;
                }
            }
        };
        __thread PerThreadState *t_perThreadState = nullptr;

        void segvHandler(int signal_number, siginfo_t *info, void *ptr)
        {
            PerThreadState *perThreadState = t_perThreadState;

            if(perThreadState)
            {
                if(perThreadState->_accessHandler(info->si_addr))
                {
                    return;
                }

                fprintf(stderr, "call SIGSEGV default handler for %p\n", info->si_addr);
                if(perThreadState->_oldAction.sa_flags & SA_SIGINFO)
                {
                    return perThreadState->_oldAction.sa_sigaction(signal_number, info, ptr);
                }
                else
                {
                    return perThreadState->_oldAction.sa_handler(signal_number);
                }
            }

            fprintf(stderr, "unable to handle SIGSEGV\n");
            return;
        }
    }


    bool threadInit(TVmAccessHandler accessHandler)
    {
        if(t_perThreadState)
        {
            fprintf(stderr, "vm::threadInit: secondary call\n");
            return false;
        }
        t_perThreadState = new PerThreadState(SIGSTKSZ);
        t_perThreadState->_accessHandler = accessHandler;

        struct sigaltstack altstack;
        memset(&altstack, 0, sizeof(altstack));
        altstack.ss_size = t_perThreadState->_altStackSize;
        altstack.ss_sp = t_perThreadState->_altStackArea;
        altstack.ss_flags = 0;
        if(sigaltstack(&altstack, &t_perThreadState->_oldAltStack))
        {
            delete t_perThreadState;
            t_perThreadState = nullptr;
            perror("sigaltstack");
            return false;
        }

        struct sigaction sa;
        memset(&sa, 0, sizeof(sa));
        sa.sa_sigaction = &segvHandler;
        sa.sa_flags = SA_SIGINFO | SA_ONSTACK;
        sigfillset (&sa.sa_mask);
        if(sigaction(SIGSEGV, &sa, &t_perThreadState->_oldAction))
        {
            delete t_perThreadState;
            t_perThreadState = nullptr;
            perror("sigaction");
            return false;
        }

        return true;

    }

    bool threadDeinit(TVmAccessHandler accessHandler)
    {
        if(!t_perThreadState)
        {
            fprintf(stderr, "vm::threadDeinit: already deinited\n");
            return false;
        }

        if(t_perThreadState->_accessHandler != accessHandler)
        {
            fprintf(stderr, "vm::threadDeinit: urong accessHandler\n");
            return false;
        }

        if(sigaction(SIGSEGV, &t_perThreadState->_oldAction, NULL))
        {
            perror("sigaction");
        }

        if(sigaltstack(&t_perThreadState->_oldAltStack, NULL))
        {
            perror("sigaltstack");
        }

        delete t_perThreadState;
        t_perThreadState = nullptr;
        return true;
    }

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

    bool free(void *addr, std::size_t size)
    {
        if(munmap(addr, size))
        {
            perror("munmap");
            return false;
        }

        return true;
    }

    bool protect(void *addr, std::size_t size, bool access)
    {
        if(mprotect(addr, size, access ? (PROT_READ|PROT_WRITE) : PROT_NONE))
        {
            perror("mprotect");
            return false;
        }

        return true;
    }

}}}}}
