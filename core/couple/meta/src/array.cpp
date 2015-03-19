#include <dci/himpl/layoutChecker.hpp>
#include <array.hpp>
#include "impl/array.hpp"

#include <type_traits>

namespace dci { namespace couple { namespace meta
{



    static_assert(0!=sizeof(himpl::Checker<Array>), "check array layout");


}}}
