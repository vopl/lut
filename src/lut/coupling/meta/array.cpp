#include "lut/stable.hpp"
#include "lut/himpl/layoutChecker.hpp"
#include "lut/coupling/meta/array.hpp"
#include "lut/coupling/meta/impl/array.hpp"

#include <type_traits>

namespace lut { namespace coupling { namespace meta
{



    static_assert(0!=sizeof(himpl::Checker<Array>), "check array layout");


}}}
