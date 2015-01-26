#include "lut/stable.hpp"
#include "lut/hiddenImpl/layoutChecker.hpp"
#include "lut/coupling/meta/array.hpp"
#include "lut/coupling/meta/impl/array.hpp"

#include <type_traits>

namespace lut { namespace coupling { namespace meta
{



    static_assert(0!=sizeof(hiddenImpl::Checker<Array>), "check array layout");


}}}
