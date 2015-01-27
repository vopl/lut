#pragma once

namespace lut { namespace himpl
{
    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class... TBaseImpls>
    class ImplLayout
        : public TBaseImpls...
    {
    public:
        using BaseImpls = std::tuple<TBaseImpls...>;
    };

}}
