#ifndef DX11RASTERIZERSTATE_H
#define DX11RASTERIZERSTATE_H

#include "Engine/Core/Render/Api/DX11/DX11Context.h"
#include "Engine/Core/Render/Api/State/RasterizerState.h"

namespace Engine {
    class DX11RasterizerState : public RasterizerState {
    private:
        DX11Context* m_dxContext;
        D3D11_RASTERIZER_DESC m_d3dRasterizerDesc;

    public:
        DX11RasterizerState(DX11Context* dxContext);
        DX11RasterizerState(const DX11RasterizerState&) = default;
        virtual ~DX11RasterizerState() = default;

        void SetCullMode(CullMode mode) override;
        void SetDepthClipEnable(bool enable) override;

        void Reset() override;

        void Bind() override;
        void UnBind() override;
    };
}

#endif // DX11RASTERIZERSTATE_H
