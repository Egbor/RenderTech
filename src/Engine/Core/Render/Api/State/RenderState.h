#ifndef RENDERSTATE_H
#define RENDERSTATE_H

#include "Engine/EngineDef.h"

namespace Engine {
    enum class StateType {
        ST_BLEND,
        ST_SAMPLER,
        ST_DEPTH_STENCIL,
        ST_RASTERIZER
    };

    class RenderState {
    public:
        RenderState() = default;
        RenderState(const RenderState&) = default;
        virtual ~RenderState() = default;

        virtual void Reset() = 0;

        virtual void Bind() = 0;
        virtual void UnBind() = 0;
    };
}

#endif // RENDERSTATE_H
