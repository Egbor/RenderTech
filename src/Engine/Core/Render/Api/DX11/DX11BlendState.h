#ifndef DX11BLENDSTATE_H
#define DX11BLENDSTATE_H

#include "Engine/Core/Render/Api/Interface/IStateResource.h"
#include "Engine/Core/Render/Api/DX11/Interface/IDX11SelfBindable.h"
#include "Engine/Core/Render/Api/DX11/DX11Context.h"

namespace Engine {
    class DX11BlendState : public IStateResourceData, public IDX11SelfBindable {
    public:
        DX11BlendState(const StateData& data, const IContext* context);
        virtual ~DX11BlendState() = default;

        void Bind(ComPtr<ID3D11DeviceContext> d3dContext) override;
        bool Is(StateType type) const override;

        ComPtr<ID3D11BlendState> GetD3D11BlendState() const;

    private:
        ComPtr<ID3D11BlendState> m_state;
        FLOAT m_blendFactor[4];
        UINT m_sampleMask;
    };
}

#endif // DX11BLENDSTATE_H
