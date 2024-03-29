#pragma once

#include <dci/himpl/implLayout.hpp>

#include <vector>

namespace dci { namespace couple { namespace meta { namespace impl
{
    template <class E>
    class Compound
        : public himpl::ImplLayout<Compound<E>>
    {
    public:
        Compound();
        ~Compound();

        void add(E *element);

    private:
        std::vector<E *> _part;
    };



    template <class E>
    Compound<E>::Compound()
    {

    }

    template <class E>
    Compound<E>::~Compound()
    {

    }

    template <class E>
    void Compound<E>::add(E *element)
    {
        _part.push_back(element);
    }


}}}}
