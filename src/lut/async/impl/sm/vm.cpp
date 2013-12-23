#include "lut/async/impl/sm/vm.hpp"

#include <unistd.h>
#include <sys/mman.h>
#include <signal.h>
#include <string.h>
#include <stdio.h>

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
        static User * g_installedUser = NULL;
        static struct sigaction g_segvOldAction;

        void segvHandler(int signal_number, siginfo_t *info, void *ptr)
        {
            fprintf(stderr, "SIGSEGV for %p\n", info->si_addr);
            fflush(stderr);

            if(g_installedUser && g_installedUser->vmHandleAccess(info->si_addr))
            {
                return;
            }

            fprintf(stderr, "call SIGSEGV default handler for %p\n", info->si_addr);

            if(g_segvOldAction.sa_flags & SA_SIGINFO)
            {
                return g_segvOldAction.sa_sigaction(signal_number, info, ptr);
            }
            else
            {
                return g_segvOldAction.sa_handler(signal_number);
            }
        }
    }


    bool startup(User *user)
    {
        if(g_installedUser)
        {
            fprintf(stderr, "vm::startup: secondary call\n");
            return false;
        }
        g_installedUser = user;

        struct sigaction sa;
        memset(&sa, 0, sizeof (sa));
        sa.sa_sigaction = &segvHandler;
        sa.sa_flags = SA_SIGINFO | SA_NODEFER | SA_ONSTACK;
        sigfillset (&sa.sa_mask);
        if(sigaction(SIGSEGV, &sa, &g_segvOldAction))
        {
            g_installedUser = nullptr;
            perror("sigaction");
            return false;
        }

        return true;

    }

    bool shutdown(User *user)
    {
        if(g_installedUser != user)
        {
            fprintf(stderr, "vm::startup: urong user\n");
            return false;
        }
        g_installedUser = nullptr;

        if(sigaction(SIGSEGV, &g_segvOldAction, NULL))
        {
            perror("sigaction");
        }

        return true;
    }

    void *alloc(size_t size)
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

    bool free(void *addr, size_t size)
    {
        if(munmap(addr, size))
        {
            perror("munmap");
            return false;
        }

        return true;
    }

    bool protect(void *addr, size_t size, bool access)
    {
        if(mprotect(addr, size, access ? (PROT_READ|PROT_WRITE) : PROT_NONE))
        {
            perror("mprotect");
            return false;
        }

        return true;
    }

}}}}}
