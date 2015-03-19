#include <mutex.hpp>
#include "impl/mutex.hpp"

namespace dci { namespace async
{
    Mutex::Mutex()
        : himpl::FaceLayout<impl::Mutex>()
    {

    }

    Mutex::~Mutex()
    {

    }

    void Mutex::lock()
    {
        return impl().lock();
    }

    bool Mutex::tryLock()
    {
        return impl().tryLock();
    }

    void Mutex::unlock()
    {
        return impl().unlock();
    }

    bool Mutex::locked() const
    {
        return impl().locked();
    }

}}
