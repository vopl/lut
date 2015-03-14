#pragma once

#include <iostream>

namespace lut { namespace logger
{

}}

#   define LOGF(msg) std::cout<<msg<<std::endl;
#   define LOGE(msg) std::cout<<msg<<std::endl;
#   define LOGW(msg) std::cout<<msg<<std::endl;
#   define LOGI(msg) std::cout<<msg<<std::endl;
#   define LOGD(msg) std::cout<<msg<<std::endl;
#   define LOGT(msg) std::cout<<msg<<std::endl;
