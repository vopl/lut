#include "lut/stable.hpp"
#include "lut/himpl/layoutChecker.hpp"
#include "lut/couple/meta/array.hpp"
#include "lut/couple/meta/impl/array.hpp"

#include <type_traits>

namespace lut { namespace couple { namespace meta
{



    static_assert(0!=sizeof(himpl::Checker<Array>), "check array layout");


}}}
