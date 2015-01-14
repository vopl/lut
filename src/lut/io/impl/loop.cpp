#include "lut/stable.hpp"
#include "lut/io/impl/loop.hpp"
#include "lut/io/error.hpp"
#include "lut/async.hpp"

#include <sys/epoll.h>

namespace lut { namespace io { namespace impl { namespace loop
{

    namespace
    {
        int g_epollfd{-1};

        fd::Base *g_activeListeners{nullptr};
    }

    fd::Base *&next(fd::Base *b)
    {
        return b->_nextOnPoller;
    }

    fd::Base *&prev(fd::Base *b)
    {
        return b->_prevOnPoller;
    }

    namespace
    {
        uint32_t mkEpollEventsMask(fd::Base *fdb)
        {
            int typeFlags = fdb->getEventTypes();

            uint32_t res = (typeFlags & fd::etf_read) ? EPOLLIN : 0;

            if(typeFlags & fd::etf_write)
            {
                res |= EPOLLOUT;
            }

            if(typeFlags & fd::etf_et)
            {
                res |= EPOLLET;
            }

            return res;
        }
    }

    std::error_code listenerAdd(fd::Base *fdb)
    {
        {
            epoll_event evt = {0};
            evt.data.ptr = fdb;
            evt.events = mkEpollEventsMask(fdb);

            if(epoll_ctl(g_epollfd, EPOLL_CTL_ADD, fdb->getDescriptor(), &evt))
            {
                return std::error_code(errno, std::system_category());
            }
        }

        {
            assert(!next(fdb));
            assert(!prev(fdb));
            if(!g_activeListeners)
            {
                g_activeListeners = fdb;
            }
            else
            {
                assert(!prev(g_activeListeners));
                prev(g_activeListeners) = fdb;
                next(fdb) = g_activeListeners;
                g_activeListeners = fdb;
            }
        }

        return std::error_code();
    }

    std::error_code listenerReplace(fd::Base *fdbFrom, fd::Base *fdbTo)
    {
        {
            if(fdbFrom->getDescriptor() != fdbTo->getDescriptor())
            {
                return make_error_code(error::general::invalid_argument);
            }

            epoll_event evt = {0};
            evt.data.ptr = fdbTo;
            evt.events = mkEpollEventsMask(fdbTo);

            if(epoll_ctl(g_epollfd, EPOLL_CTL_MOD, fdbFrom->getDescriptor(), &evt))
            {
                return std::error_code(errno, std::system_category());
            }
        }

        {
            assert(!next(fdbTo));
            assert(!prev(fdbTo));

            if(next(fdbFrom))
            {
                prev(next(fdbFrom)) = fdbTo;
            }
            if(prev(fdbFrom))
            {
                next(prev(fdbFrom)) = fdbTo;
            }

            if(fdbFrom == g_activeListeners)
            {
                g_activeListeners = fdbTo;
            }

            std::swap(next(fdbFrom), next(fdbTo));
            std::swap(prev(fdbFrom), prev(fdbTo));
        }

        return std::error_code();
    }

    std::error_code listenerDel(fd::Base *fdb)
    {
        {
            if(epoll_ctl(g_epollfd, EPOLL_CTL_DEL, fdb->getDescriptor(), nullptr))
            {
                return std::error_code(errno, std::system_category());
            }
        }

        {
            if(fdb == g_activeListeners)
            {
                if(prev(fdb))
                {
                    g_activeListeners = prev(fdb);
                }
                else
                {
                    g_activeListeners = next(fdb);
                }
            }

            if(next(fdb))
            {
                prev(next(fdb)) = prev(fdb);
            }
            if(prev(fdb))
            {
                next(prev(fdb)) = next(fdb);
            }

            next(fdb) = nullptr;
            prev(fdb) = nullptr;
        }

        return std::error_code();
    }

    std::error_code run()
    {
        if(-1 != g_epollfd)
        {
            return make_error_code(error::general::already_runned);
        }

        g_epollfd = epoll_create(1);
        if(-1 == g_epollfd)
        {
            return std::error_code(errno, std::system_category());
        }

        std::error_code res;
        epoll_event eventsBuffer[128];

        for(;;)
        {
            async::executeReadyCoros();

            int eventsAmount = epoll_wait(g_epollfd, eventsBuffer, sizeof(eventsBuffer)/sizeof(eventsBuffer[0]), -1);

            if(-1 == eventsAmount)
            {
                if(-1 == g_epollfd)
                {
                    res = std::error_code();
                    break;
                }

                res = std::error_code(errno, std::system_category());
                break;
            }

            for(int idx(0); idx<eventsAmount; ++idx)
            {
                epoll_event &evt = eventsBuffer[idx];

                fd::Base *fdb = static_cast<fd::Base *>(evt.data.ptr);

                int typeFlags =
                        ((evt.events & (EPOLLIN|EPOLLPRI)) ? (int)fd::etf_read : 0) |
                        ((evt.events & (EPOLLOUT)) ? (int)fd::etf_write : 0) |
                        ((evt.events & (EPOLLERR|EPOLLHUP)) ? (int)fd::etf_error : 0) |
                        0;

                fdb->fdEvent(typeFlags);
            }
        }

        if(-1 != g_epollfd)
        {
            close(g_epollfd);
            g_epollfd = -1;
        }

        async::executeReadyCoros();

        return res;
    }

    std::error_code stop()
    {
        if(-1 == g_epollfd)
        {
            return make_error_code(error::general::not_runned);
        }

        if(g_activeListeners)
        {
            fd::Base *listener = g_activeListeners;
            g_activeListeners = nullptr;

            do
            {
                assert(!prev(listener));


                fd::Base *cur = listener;

                listener = next(listener);

                cur->fdClose();
            }
            while(listener);
        }

        close(g_epollfd);
        g_epollfd = -1;

        return std::error_code();
    }

}}}}
