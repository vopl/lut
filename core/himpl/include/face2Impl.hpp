#pragma once

namespace dci { namespace himpl
{
    template <class TFace>
    typename TFace::Impl *face2Impl(TFace *face)
    {
        return reinterpret_cast<typename TFace::Impl *>(face);
    }

    template <class TFace>
    typename TFace::Impl &face2Impl(TFace &face)
    {
        return reinterpret_cast<typename TFace::Impl &>(face);
    }

    template <class TFace>
    const typename TFace::Impl *face2Impl(const TFace *face)
    {
        return reinterpret_cast<const typename TFace::Impl *>(face);
    }

    template <class TFace>
    const typename TFace::Impl &face2Impl(const TFace &face)
    {
        return reinterpret_cast<const typename TFace::Impl &>(face);
    }

}}
