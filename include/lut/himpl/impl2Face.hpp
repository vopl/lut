#pragma once

namespace lut { namespace himpl
{
    template <class TFace>
    TFace *impl2Face(typename TFace::Impl *impl)
    {
        return reinterpret_cast<TFace *>(impl);
    }

    template <class TFace>
    TFace &impl2Face(typename TFace::Impl &impl)
    {
        return reinterpret_cast<TFace &>(impl);
    }

    template <class TFace>
    const TFace *impl2Face(const typename TFace::Impl *impl)
    {
        return reinterpret_cast<const TFace *>(impl);
    }

    template <class TFace>
    const TFace &impl2Face(const typename TFace::Impl &impl)
    {
        return reinterpret_cast<const TFace &>(impl);
    }

}}
