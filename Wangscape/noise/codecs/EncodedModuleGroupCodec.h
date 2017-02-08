#pragma once

#include <spotify/json.hpp>

#include "noise/EncodedModuleGroup.h"

namespace spotify
{
namespace json
{

template<>
struct default_codec_t<noise::EncodedModuleGroup>
{
    static codec::object_t<noise::EncodedModuleGroup> codec()
    {
        auto codec = codec::object<noise::EncodedModuleGroup>();
        codec.required("Modules", &noise::EncodedModuleGroup::encodedModules);
        codec.optional("InputModules",
                       [](const noise::EncodedModuleGroup& emg) { return emg.moduleGroup->inputModules; },
                       [](noise::EncodedModuleGroup& emg,std::vector<noise::ModuleGroup::ModuleID> input_modules){ emg.moduleGroup->inputModules = input_modules; });
        codec.optional("OutputModule",
                       [](const noise::EncodedModuleGroup& emg) { return emg.moduleGroup->outputModule; },
                       [](noise::EncodedModuleGroup& emg, noise::ModuleGroup::ModuleID output_module) { emg.moduleGroup->outputModule = output_module; });

        return codec;
    }
};

}
}
