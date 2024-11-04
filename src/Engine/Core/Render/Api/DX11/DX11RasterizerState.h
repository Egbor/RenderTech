#ifndef DX11RASTERIZERSTATE_H
#define DX11RASTERIZERSTATE_H

#include "Engine/Core/Render/Base/RenderBase.h"
#include "Engine/Core/Render/Base/Resource/StateResource.h"

namespace Engine {
    class DX11RasterizerState : public StandaloneStateResource {
    public:
        DX11RasterizerState(const StateData& data, IContext* context);
        virtual ~DX11RasterizerState() = default;

        void Bind() const override;
        bool Is(StateType type) const override;

        ComPtr<ID3D11RasterizerState> GetD3D11RasterizerState() const;

    private:
        ComPtr<ID3D11RasterizerState> m_state;
    };
}

#endif // DX11RASTERIZERSTATE_H
