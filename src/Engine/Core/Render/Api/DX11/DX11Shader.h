#ifndef DX11SHADER_H
#define DX11SHADER_H

#include "Engine/Core/Render/Api/DX11/DX11Def.h"
#include "Engine/Core/Render/Api/Interface/IShaderResource.h"

namespace Engine {
    class DX11VertexShader : public IShaderResourceData {
    public:
        DX11VertexShader(ComPtr<ID3D11Device> d3dDevice, Size codeLength, const void* code);
        virtual ~DX11VertexShader() = default;

        bool Is(ShaderType type) const override;

        ComPtr<ID3D11InputLayout> GetD3D11Layout() const;
        ComPtr<ID3D11VertexShader> GetD3D11Shader() const;

    private:
        ComPtr<ID3D11InputLayout> m_d3dLayout;
        ComPtr<ID3D11VertexShader> m_d3dShader;
    };

    class DX11PixelShader : public IShaderResourceData {
    public:
        DX11PixelShader(ComPtr<ID3D11Device> d3dDevice, Size codeLength, const void* code);
        virtual ~DX11PixelShader() = default;

        bool Is(ShaderType type) const override;

        ComPtr<ID3D11PixelShader> GetD3D11Shader() const;

    private:
        ComPtr<ID3D11PixelShader> m_d3dShader;
    };
}

#endif // DX11SHADER_H
