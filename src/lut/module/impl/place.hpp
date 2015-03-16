#pragma once

#include "lut/himpl/implLayout.hpp"

namespace lut { namespace module { namespace impl
{
    class Place
        : public himpl::ImplLayout<Place>
    {
    public:
        Place();
        Place(const std::string &dir);
        ~Place();

        const std::string &getDir() const;
        void setDir(const std::string &dir);

    private:
        std::string _dir;
    };
}}}
