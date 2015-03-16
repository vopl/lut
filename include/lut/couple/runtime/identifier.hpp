#pragma once

#include <cstddef>
#include <string>

namespace lut { namespace couple { namespace runtime
{
    template <class Tag, std::size_t size=128/8>
    class Identifier
    {
        //not impl
    public:
        Identifier();
        ~Identifier();

        bool fromHex(const std::string &txt);

    private:
        unsigned char _data[size] = {};
    };

    template <class Tag, std::size_t size>
    Identifier<Tag, size>::Identifier()
    {
    }

    template <class Tag, std::size_t size>
    Identifier<Tag, size>::~Identifier()
    {
    }

    template <class Tag, std::size_t size>
    bool Identifier<Tag, size>::fromHex(const std::string &txt)
    {
        if(txt.size() < size*2)
        {
            return false;
        }

        for(std::size_t i(0); i<size; ++i)
        {
            unsigned int h = static_cast<unsigned char>(txt[i*2+0]);
            unsigned int l = static_cast<unsigned char>(txt[i*2+1]);

            if(h >= 'a' && h <='f') h -= 'a'-10;
            else if(h >= 'A' && h <='F') h -= 'A'-10;
            else if(h >= '0' && h <='9') h -= '0';
            else return false;

            if(l >= 'a' && l <='f') l -= 'a'-10;
            else if(l >= 'A' && l <='F') l -= 'A'-10;
            else if(l >= '0' && l <='9') l -= '0';
            else return false;

            _data[i] = h<<4 | l;
        }

        return true;
    }

}}}
