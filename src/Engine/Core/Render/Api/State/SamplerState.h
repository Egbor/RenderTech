#ifndef SAMPLERSTATE_H
#define SAMPLERSTATE_H

#include "Engine/Core/Render/Api/State/RenderState.h"

namespace Engine {
    enum class SamplerAddress {
        SA_WRAP	= 0,
        SA_MIRROR = 1,
        SA_CLAMP = 2,
        SA_BORDER = 3,
        SA_MIRROR_ONCE = 4
    };

    class SamplerState : public RenderState {
    public:
        SamplerState() = default;
        SamplerState(const SamplerState&) = default;
        virtual ~SamplerState() = default;

        virtual void SetAddress(SamplerAddress address) = 0;

        static StateType Type() {
            return StateType::ST_SAMPLER;
        }
    };
}

#endif // SAMPLERSTATE_H
