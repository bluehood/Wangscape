#pragma once

#include <spotify/json.hpp>

#include <noise/noise.h>
#include "noise/module/Wrapper.h"
#include "noise/module/Pow.h"

namespace spotify
{
namespace json
{

template<>
struct default_codec_t<noise::module::Wrapper<noise::module::Pow>>
{
    using PowWrapper = noise::module::Wrapper<noise::module::Pow>;
    static codec::object_t<PowWrapper> codec();
};
}
}
