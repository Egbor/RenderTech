#ifndef DX11TARGET_H
#define DX11TARGET_H

#include "Engine/Object/Class/Target.h"
#include "Engine/Core/Render/Api/DX11/DX11Texture.h"

namespace Engine {
    class DX11Target : public Target {
        GENERATE_BODY(DX11Target, Target);

    private:
        Texture2D* m_dxTexture;

    public:
        DX11Target(const ObjectArgument& argument);
        DX11Target(const DX11Target&) = default;
        virtual ~DX11Target();

        virtual ComPtr<ID3D11View> GetD3D11View() const;

        Texture2D* GetTexture2D() const override;
        void Clear(TargetClear clear) override;

        virtual void Create(Texture2D* texture);

    protected:
        template<class Texture2DClass, class Predicate>
        static void CreateD3D11View(DX11Target* owner, DX11Texture2DData* data, Predicate creator) {
            if (creator()) {
                owner->m_dxTexture = owner->CreateDefaultSubobject<Texture2DClass>();
                owner->m_dxTexture->As<Texture2DClass>()->Data().Initialize(data->GetD3D11Texture2D());
            }
        }
    };

    class DX11OutputTarget : public DX11Target {
        GENERATE_BODY(DX11OutputTarget, DX11Target);

    private:
        ComPtr<ID3D11RenderTargetView> m_d3dView;

    public:
        DX11OutputTarget(const ObjectArgument& argument);
        DX11OutputTarget(const DX11OutputTarget&) = default;
        virtual ~DX11OutputTarget() = default;

        ComPtr<ID3D11View> GetD3D11View() const override;

        void Clear(TargetClear clear) override;
        void Create(Texture2D* texture) override;

    protected:
        static bool CreateD3D11RenderTargetView(ComPtr<ID3D11Device> d3dDevice,
                                                ComPtr<ID3D11Texture2D> d3dTexture2D,
                                                D3D11_RENDER_TARGET_VIEW_DESC* d3dRenderTargetViewDesc,
                                                DX11OutputTarget* owner);
    };

    class DX11DepthStencilTarget : public DX11Target {
        GENERATE_BODY(DX11DepthStencilTarget, DX11Target);

    private:
        ComPtr<ID3D11DepthStencilView> m_d3dView;

    public:
        DX11DepthStencilTarget(const ObjectArgument& argument);
        DX11DepthStencilTarget(const DX11DepthStencilTarget&) = default;
        virtual ~DX11DepthStencilTarget() = default;

        ComPtr<ID3D11View> GetD3D11View() const override;

        void Clear(TargetClear clear) override;
        void Create(Texture2D* texture) override;

    protected:
        static bool CreateD3D11DepthStencilView(ComPtr<ID3D11Device> d3dDevice,
                                                ComPtr<ID3D11Texture2D> d3dTexture2D,
                                                D3D11_DEPTH_STENCIL_VIEW_DESC* d3dDepthStencilViewDesc,
                                                DX11DepthStencilTarget* owner);
    };

    class DX11OutputCubeTarget : public DX11OutputTarget {
        GENERATE_BODY(DX11OutputCubeTarget, DX11OutputTarget)

    public:
        DX11OutputCubeTarget(const ObjectArgument& argument);
        DX11OutputCubeTarget(const DX11OutputCubeTarget&) = default;
        virtual ~DX11OutputCubeTarget() = default;

        void Create(Texture2D* texture) override;
    };
}

#endif // DX11TARGET_H
