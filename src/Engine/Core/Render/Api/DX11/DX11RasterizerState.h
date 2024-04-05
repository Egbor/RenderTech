#ifndef DX11RASTERIZERSTATE_H
#define DX11RASTERIZERSTATE_H

#include "Engine/Core/Render/Api/Interface/IStateResource.h"
#include "Engine/Core/Render/Api/DX11/Interface/IDX11SelfBindable.h"
#include "Engine/Core/Render/Api/DX11/DX11Context.h"

namespace Engine {
    class DX11RasterizerState : public IStateResourceData, public IDX11SelfBindable {
    public:
        DX11RasterizerState(const StateData& data, const IContext* context);
        virtual ~DX11RasterizerState() = default;

        void Bind(ComPtr<ID3D11DeviceContext> d3dContext) override;
        bool Is(StateType type) const override;

        ComPtr<ID3D11RasterizerState> GetD3D11RasterizerState() const;

    private:
        ComPtr<ID3D11RasterizerState> m_state;
    };
}

#endif // DX11RASTERIZERSTATE_H
