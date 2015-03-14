#pragma once

#include "lut/couple/runtime/identifier.hpp"


namespace lut { namespace module
{
    struct ModuleIdentifierTag;
    using Mid = couple::runtime::Identifier<ModuleIdentifierTag>;
}}
