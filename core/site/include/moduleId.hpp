#pragma once

#include <dci/couple/runtime/identifier.hpp>


namespace dci { namespace site
{
    struct ModuleIdentifierTag;
    using Mid = couple::runtime::Identifier<ModuleIdentifierTag>;
}}
