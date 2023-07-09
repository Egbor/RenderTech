#ifndef DX11SHADER_H
#define DX11SHADER_H

#include "Engine/Object/Class/Shader.h"
#include "Engine/Core/Render/Api/DX11/DX11Context.h"

namespace Engine {
    CLASSTYPE(DX11Shader)
    class DX11Shader : public Shader {
        GENERATE_BODY(DX11Shader, Shader)

    public:
        DX11Shader(const ObjectArgument& argument);
        DX11Shader(const DX11Shader&) = default;
        virtual ~DX11Shader() = default;

        virtual void Bind(const DX11Context* context);
        virtual void UnBind(const DX11Context* context);

        virtual void Create(const DX11Context* context, const void* code, Size codeLength);
    };

    CLASSTYPE(DX11VertexShader)
    class DX11VertexShader : public DX11Shader {
        GENERATE_BODY(DX11VertexShader, DX11Shader)

    private:
        ComPtr<ID3D11VertexShader> m_d3dVertexShader;
        ComPtr<ID3D11InputLayout> m_d3dInputLayout;

    public:
        DX11VertexShader(const ObjectArgument& argument);
        DX11VertexShader(const DX11VertexShader&) = default;
        virtual ~DX11VertexShader() = default;

        void Create(const DX11Context* context, const void* code, Size codeLength) override;

    private:
        void Bind(const DX11Context* context) override;
        void UnBind(const DX11Context* context) override;
    };

    CLASSTYPE(DX11PixelShader)
    class DX11PixelShader : public DX11Shader {
        GENERATE_BODY(DX11PixelShader, DX11Shader)

    private:
        ComPtr<ID3D11PixelShader> m_d3dPixelShader;

    public:
        DX11PixelShader(const ObjectArgument& argument);
        DX11PixelShader(const DX11PixelShader&) = default;
        virtual ~DX11PixelShader() = default;

        void Create(const DX11Context* context, const void* code, Size codeLength) override;

    private:
        void Bind(const DX11Context* context) override;
        void UnBind(const DX11Context* context) override;
    };
}

#endif // DX11SHADER_H
