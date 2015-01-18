#pragma once

namespace lut { namespace hiddenImpl
{
    class Accessor;

    template <class FaceBase, class DerivedImpl>
    class Compound
        : public FaceBase
    {
    protected:
        friend class hiddenImpl::Accessor;

        DerivedImpl *pimpl();
        DerivedImpl &impl();

        const DerivedImpl *pimpl() const;
        const DerivedImpl &impl() const;

    private:
        char _data[hiddenImpl::sizeProvider<DerivedImpl>::_value - hiddenImpl::sizeProvider<typename FaceBase::Impl>::_value];
    };


}}
