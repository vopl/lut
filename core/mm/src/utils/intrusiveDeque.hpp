#pragma once

namespace dci { namespace mm { namespace utils
{
    template <typename T>
    class IntrusiveDeque
    {
    public:
        IntrusiveDeque();
        IntrusiveDeque(T *element);
        ~IntrusiveDeque();

    public:
        bool empty() const;
        T *first() const;
        T *last() const;
        bool contain(T *element) const;
        void push(T *element);
        void remove(T *element);

    private:
        T *_first;
        T *_last;
    };
}}}
