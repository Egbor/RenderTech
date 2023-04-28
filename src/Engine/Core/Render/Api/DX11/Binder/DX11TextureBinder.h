#ifndef DX11TEXTUREBINDER_H
#define DX11TEXTUREBINDER_H

#include "Engine/Core/Render/Api/RenderBinder.h"
#include "Engine/Core/Render/Api/DX11/DX11Texture.h"

namespace Engine {
    class DX11TextureBinder : public RenderBinder<TextureSlot, Texture2D> {
    private:
        DX11Context* m_dxContext;
        Array<ComPtr<ID3D11ShaderResourceView>> m_dxTextures;

    public:
        DX11TextureBinder(DX11Context* context);
        DX11TextureBinder(const DX11TextureBinder&) = default;
        virtual ~DX11TextureBinder() = default;

        void Bind() override;
        void UnBind() override;

        void AttachRenderItem(TextureSlot id, Texture2D* item) override;
        void DetachRenderItem(TextureSlot id) override;
    };
}

#endif // DX11TEXTUREBINDER_H
