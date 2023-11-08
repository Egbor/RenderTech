#include "Engine/Core/Render/Api/DX11/DX11Shader.h"
#include "Engine/Core/System/Exception/EngineException.h"

namespace Engine {
	DX11VertexShader::DX11VertexShader(ComPtr<ID3D11Device> d3dDevice, Size codeLength, const void* code) {
		HRESULT hr = 0;
		if (FAILED(hr = d3dDevice->CreateVertexShader(code, codeLength, nullptr, &m_d3dShader))) {
			throw EngineException("[DX11VertexShader] ID3D11Device::CreateVertexShader() failed");
		}
		if (FAILED(hr = d3dDevice->CreateInputLayout(D3D11InputDesc, ARRAYSIZE(D3D11InputDesc), code, codeLength, &m_d3dLayout))) {
			throw EngineException("[DX11VertexShader] ID3D11Device::CreateInputLayout() failed");
		}
	}

	bool DX11VertexShader::Is(ShaderType type) const {
		return type == ShaderType::ST_VERTEX;
	}

	ComPtr<ID3D11VertexShader> DX11VertexShader::GetD3D11Shader() const {
		return m_d3dShader;
	}

	ComPtr<ID3D11InputLayout> DX11VertexShader::GetD3D11Layout() const {
		return m_d3dLayout;
	}

	DX11PixelShader::DX11PixelShader(ComPtr<ID3D11Device> d3dDevice, Size codeLength, const void* code) {
		HRESULT hr = 0;
		if (FAILED(hr = d3dDevice->CreatePixelShader(code, codeLength, nullptr, &m_d3dShader))) {
			throw EngineException("[DX11PixelShader] ID3D11Device::CreatePixelShader() fialed");
		}
	}

	bool DX11PixelShader::Is(ShaderType type) const {
		return type == ShaderType::ST_PIXEL;
	}

	ComPtr<ID3D11PixelShader> DX11PixelShader::GetD3D11Shader() const {
		return m_d3dShader;
	}
}
