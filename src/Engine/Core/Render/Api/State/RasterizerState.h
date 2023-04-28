#ifndef RASTERIZERSTATE_H
#define RASTERIZERSTATE_H

#include "Engine/Core/Render/Api/State/RenderState.h"

namespace Engine {
    enum class CullMode {
        D3D11_CULL_NONE = 0,
        D3D11_CULL_FRONT = 1,
        D3D11_CULL_BACK = 2
    };

    class RasterizerState : public RenderState {
    public:
        RasterizerState() = default;
        RasterizerState(const RasterizerState&) = default;
        virtual ~RasterizerState() = default;

        virtual void SetCullMode(CullMode mode) = 0;
        virtual void SetDepthClipEnable(bool enable) = 0;

        static StateType Type() {
            return StateType::ST_RASTERIZER;
        }
    };
}

#endif // RASTERIZERSTATE_H
