#include "Engine/Core/Render/Api/DX11/DX11Shader.h"
#include "Engine/Core/System/Exception/EngineException.h"

namespace Engine {
    //GENERATE_RTTI_DEFINITIONS(DX11Shader)
    GENERATE_INSTANTIATION(DX11Shader)

    DX11Shader::DX11Shader(const ObjectArgument& argument)
        : Super(argument) {

    }

    void DX11Shader::Bind(const DX11Context* context) {
        throw new EngineException("[DX11Shader] DX11Shader::Bind() is abstract method without any features");
    }

    void DX11Shader::UnBind(const DX11Context* context) {
        throw new EngineException("[DX11Shader] DX11Shader::UnBind() is abstract method without any features");
    }

    void DX11Shader::Create(const DX11Context* context, const void* code, Size codeLength) {
        throw new EngineException("[DX11Shader] DX11Shader::Create() is abstract method without any features");
    }

    //GENERATE_RTTI_DEFINITIONS(DX11VertexShader)
    GENERATE_INSTANTIATION(DX11VertexShader)

    DX11VertexShader::DX11VertexShader(const ObjectArgument& argument)
        : Super(argument) {

    }

    void DX11VertexShader::Create(const DX11Context* context, const void* code, Size codeLength) {
        HRESULT hr = 0;
        ComPtr<ID3D11Device> d3dDevice = context->GetD3D11Device();

        if (FAILED(hr = d3dDevice->CreateInputLayout(D3D11InputDesc, ARRAYSIZE(D3D11InputDesc), code, codeLength, &m_d3dInputLayout))) {
            throw new EngineException("[DX11VertexShader] ID3D11Device::CreateInputLayout() failed");
        }

        if (FAILED(hr = d3dDevice->CreateVertexShader(code, codeLength, NULL, &m_d3dVertexShader))) {
            throw new EngineException("[DX11VertexShader] ID3D11Device::CreateVertexShader() failed");
        }
    }

    void DX11VertexShader::Bind(const DX11Context* context) {
        ComPtr<ID3D11DeviceContext> d3dContext = context->GetD3D11Context();
        d3dContext->IASetInputLayout(m_d3dInputLayout.Get());
        d3dContext->VSSetShader(m_d3dVertexShader.Get(), NULL, 0);
    }

    void DX11VertexShader::UnBind(const DX11Context* context) {
        ComPtr<ID3D11DeviceContext> d3dContext = context->GetD3D11Context();
        d3dContext->VSSetShader(NULL, NULL, 0);
    }

    //GENERATE_RTTI_DEFINITIONS(DX11PixelShader)
    GENERATE_INSTANTIATION(DX11PixelShader)

    DX11PixelShader::DX11PixelShader(const ObjectArgument& argument)
        : Super(argument) {

    }

    void DX11PixelShader::Create(const DX11Context* context, const void* code, Size codeLength) {
        HRESULT hr = 0;
        ComPtr<ID3D11Device> d3dDevice = context->GetD3D11Device();

        if (FAILED(hr = d3dDevice->CreatePixelShader(code, codeLength, NULL, &m_d3dPixelShader))) {
            throw new EngineException("[DX11PixelShader] ID3D11Device::CreatePixelShader() failed");
        }
    }

    void DX11PixelShader::Bind(const DX11Context* context) {
        ComPtr<ID3D11DeviceContext> d3dContext = context->GetD3D11Context();
        d3dContext->PSSetShader(m_d3dPixelShader.Get(), NULL, 0);
    }

    void DX11PixelShader::UnBind(const DX11Context* context) {
        ComPtr<ID3D11DeviceContext> d3dContext = context->GetD3D11Context();
        d3dContext->PSSetShader(NULL, NULL, 0);
    }
}
