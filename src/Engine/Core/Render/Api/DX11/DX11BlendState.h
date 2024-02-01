#ifndef DX11BLENDSTATE_H
#define DX11BLENDSTATE_H

#include "Engine/Core/Render/Api/Interface/IStateResource.h"
#include "Engine/Core/Render/Api/DX11/DX11Context.h"

namespace Engine {
    class DX11BlendStateData : public IBlendStateData {
    private:
        D3D11_BLEND_DESC m_d3dBlendDesc;

    public:
        DX11BlendStateData();
        virtual ~DX11BlendStateData() = default;

        void Reset() override;

        void SetSourceFactor(RenderOutput target, Blend factor) override;
        void SetDestinationFactor(RenderOutput target, Blend factor) override;
        void SetOperation(RenderOutput target, BlendOperation operation) override;

        void SetEnable(RenderOutput target, bool enable) override;

        const D3D11_BLEND_DESC& GetD3D11Desc() const;
    };

    class DX11BlendState : public IStateResourceData {
    public:
        DX11BlendState(const IStateData* data, const IContext* context);
        virtual ~DX11BlendState() = default;

        void Initialize(const IStateData* data, const IContext* context) override;

        ComPtr<ID3D11BlendState> GetD3D11BlendState() const;

    private:
        ComPtr<ID3D11BlendState> m_state;
    };
}

#endif // DX11BLENDSTATE_H
