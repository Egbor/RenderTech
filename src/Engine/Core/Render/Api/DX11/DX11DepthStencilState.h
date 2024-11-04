#ifndef DX11DEPTHSTENCILSTATE_H
#define DX11DEPTHSTENCILSTATE_H

#include "Engine/Core/Render/Api/DX11/DX11Def.h"
#include "Engine/Core/Render/Base/Resource/StateResource.h"

namespace Engine {
    class DX11DepthStencilState : public StandaloneStateResource {
    public:
        DX11DepthStencilState(const StateData& data, IContext* context);
        virtual ~DX11DepthStencilState() = default;

        void Bind() const override;
        bool Is(StateType type) const override;

        ComPtr<ID3D11DepthStencilState> GetD3D11DepthStencilState() const;

    private:
        ComPtr<ID3D11DepthStencilState> m_state;
        UINT m_stencilRef;
    };
}

#endif // DX11DEPTHSTENCILSTATE_H
