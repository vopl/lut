#pragma once

namespace lut { namespace hiddenImpl
{
    class Accessor;

    ////////////////////////////////////////////////////////////////////////////////
    template <class FaceBase, class DerivedImpl>
    class Compound
        : public FaceBase
    {
    public:
        using Impl = DerivedImpl;

    protected:
        friend class hiddenImpl::Accessor;

        DerivedImpl *pimpl();
        DerivedImpl &impl();

        const DerivedImpl *pimpl() const;
        const DerivedImpl &impl() const;

    private:
        char _data[hiddenImpl::sizeProvider<DerivedImpl>::_value - hiddenImpl::sizeProvider<typename FaceBase::Impl>::_value];
    };

    ////////////////////////////////////////////////////////////////////////////////
    template <class FaceBase1, class FaceBase2, class DerivedImpl>
    class Compound2
        : public FaceBase1
        , public FaceBase2
    {
    public:
        using Impl = DerivedImpl;

    protected:
        friend class hiddenImpl::Accessor;

        DerivedImpl *pimpl();
        DerivedImpl &impl();

        const DerivedImpl *pimpl() const;
        const DerivedImpl &impl() const;

    private:
        char _data[
            hiddenImpl::sizeProvider<DerivedImpl>::_value
            - hiddenImpl::sizeProvider<typename FaceBase1::Impl>::_value
            - hiddenImpl::sizeProvider<typename FaceBase2::Impl>::_value
        ];
    };

    ////////////////////////////////////////////////////////////////////////////////
    template <class FaceBase1, class FaceBase2, class FaceBase3, class DerivedImpl>
    class Compound3
        : public FaceBase1
        , public FaceBase2
        , public FaceBase3
    {
    public:
        using Impl = DerivedImpl;

    protected:
        friend class hiddenImpl::Accessor;

        DerivedImpl *pimpl();
        DerivedImpl &impl();

        const DerivedImpl *pimpl() const;
        const DerivedImpl &impl() const;

    private:
        char _data[
            hiddenImpl::sizeProvider<DerivedImpl>::_value
            - hiddenImpl::sizeProvider<typename FaceBase1::Impl>::_value
            - hiddenImpl::sizeProvider<typename FaceBase2::Impl>::_value
            - hiddenImpl::sizeProvider<typename FaceBase3::Impl>::_value
        ];
    };













}}
