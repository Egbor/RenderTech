#ifndef DX11SAMPLERSTATE_H
#define DX11SAMPLERSTATE_H

#include "Engine/Core/Render/Api/DX11/DX11Def.h"
#include "Engine/Core/Render/Base/Resource/StateResource.h"

namespace Engine {
    class DX11SamplerState : public StateResource {
    public:
        DX11SamplerState(const StateData& data, IContext* context);
        virtual ~DX11SamplerState() = default;

        bool Is(StateType type) const override;

        ComPtr<ID3D11SamplerState> GetD3D11SamplerState() const;

    private:
        ComPtr<ID3D11SamplerState> m_state;
    };
}

#endif // DX11SAMPLERSTATE_H
