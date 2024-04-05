#ifndef DX11DEPTHSTENCILSTATE_H
#define DX11DEPTHSTENCILSTATE_H

#include "Engine/Core/Render/Api/Interface/IStateResource.h"
#include "Engine/Core/Render/Api/DX11/Interface/IDX11SelfBindable.h"
#include "Engine/Core/Render/Api/DX11/DX11Context.h"

namespace Engine {
    class DX11DepthStencilState : public IStateResourceData, public IDX11SelfBindable {
    public:
        DX11DepthStencilState(const StateData& data, const IContext* context);
        virtual ~DX11DepthStencilState() = default;

        void Bind(ComPtr<ID3D11DeviceContext> d3dContext) override;
        bool Is(StateType type) const override;

        ComPtr<ID3D11DepthStencilState> GetD3D11DepthStencilState() const;

    private:
        ComPtr<ID3D11DepthStencilState> m_state;
        UINT m_stencilRef;
    };
}

#endif // DX11DEPTHSTENCILSTATE_H
