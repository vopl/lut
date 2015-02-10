#pragma once

#include "lut/himpl/implLayout.hpp"

namespace lut { namespace coupling { namespace meta { namespace impl
{
    template <class B>
    class Inheritable
        : public himpl::ImplLayout<>
    {
    public:
        Inheritable();
        ~Inheritable();

        void addBase(B *);
    };



    template <class B>
    Inheritable<B>::Inheritable()
    {
    }

    template <class B>
    Inheritable<B>::~Inheritable()
    {
    }

    template <class B>
    void Inheritable<B>::addBase(B *)
    {
        assert(0);
    }

}}}}
