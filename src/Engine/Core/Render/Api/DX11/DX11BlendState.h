#ifndef DX11BLENDSTATE_H
#define DX11BLENDSTATE_H

#include "Engine/Core/Render/Api/DX11/DX11Def.h"
#include "Engine/Core/Render/Base/Resource/StateResource.h"

namespace Engine {
    class DX11BlendState : public StandaloneStateResource {
    public:
        DX11BlendState(const String& name, const StateData& data, IContext* context);
        virtual ~DX11BlendState() = default;

        void Bind() const override;
        bool Is(StateType type) const override;

        ComPtr<ID3D11BlendState> GetD3D11BlendState() const;

    private:
        ComPtr<ID3D11BlendState> m_state;
        FLOAT m_blendFactor[4];
        UINT m_sampleMask;
    };
}

#endif // DX11BLENDSTATE_H
