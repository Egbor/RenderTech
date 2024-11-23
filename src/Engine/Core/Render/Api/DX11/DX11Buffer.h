#ifndef DX11BUFFER_H
#define DX11BUFFER_H

#include "Engine/Core/Render/Api/DX11/DX11Def.h"
#include "Engine/Core/Render/Base/Resource/BufferResource.h"

namespace Engine {
    class DX11Buffer : public BufferResource {
    public:
        DX11Buffer(const String& name, IContext* context, D3D11_USAGE usage, D3D11_BIND_FLAG bindFlags, 
            UINT cpuAccessFlags, UINT size, UINT strides, const void* data);
        virtual ~DX11Buffer() = default;

        Int32 GetNumberOfBytes() const override;
        Int32 GetNumberOfElements() const override;
        RawData& GetBufferData() override;

        void Update() override;

        ComPtr<ID3D11Buffer> GetD3D11Buffer() const;
        const UINT* GetStrides() const;
        const UINT* GetOffset() const;

    protected:
        void* Lock(ComPtr<ID3D11DeviceContext> d3dContext);
        void Unlock(ComPtr<ID3D11DeviceContext> d3dContext);

    private:
        RawData m_data;
        ComPtr<ID3D11Buffer> m_d3dBuffer;

        UINT m_strides;
        UINT m_offset;
        bool m_isMutable;
    };
}

#endif // DX11BUFFER_H
