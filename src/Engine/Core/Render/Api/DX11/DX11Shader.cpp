#include "Engine/Core/Render/Api/DX11/DX11Shader.h"
#include "Engine/Core/Render/Api/DX11/DX11Context.h"
#include "Engine/Core/System/Exception/EngineException.h"

namespace Engine {
	DX11VertexShader::DX11VertexShader(IContext* context, Size codeLength, const void* code) 
		: RenderBase(RenderIdentifier::RI_SHADER, context) {
		DX11Context* dxContext = dynamic_cast<DX11Context*>(context);
		ComPtr<ID3D11Device> d3dDevice = dxContext->GetD3D11Device();

		HRESULT hr = 0;
		if (FAILED(hr = d3dDevice->CreateVertexShader(code, codeLength, nullptr, &m_d3dShader))) {
			throw EngineException("[DX11VertexShader] ID3D11Device::CreateVertexShader() failed");
		}
		if (FAILED(hr = d3dDevice->CreateInputLayout(D3D11InputDesc, ARRAYSIZE(D3D11InputDesc), code, codeLength, &m_d3dLayout))) {
			throw EngineException("[DX11VertexShader] ID3D11Device::CreateInputLayout() failed");
		}
	}

	bool DX11VertexShader::Is(RenderStage stage) const {
		return stage == RenderStage::RS_VERTEX;
	}

	ComPtr<ID3D11VertexShader> DX11VertexShader::GetD3D11Shader() const {
		return m_d3dShader;
	}

	ComPtr<ID3D11InputLayout> DX11VertexShader::GetD3D11Layout() const {
		return m_d3dLayout;
	}

	DX11PixelShader::DX11PixelShader(IContext* context, Size codeLength, const void* code) 
		: RenderBase(RenderIdentifier::RI_SHADER, context) {
		DX11Context* dxContext = dynamic_cast<DX11Context*>(context);
		ComPtr<ID3D11Device> d3dDevice = dxContext->GetD3D11Device();

		HRESULT hr = 0;
		if (FAILED(hr = d3dDevice->CreatePixelShader(code, codeLength, nullptr, &m_d3dShader))) {
			throw EngineException("[DX11PixelShader] ID3D11Device::CreatePixelShader() fialed");
		}
	}

	bool DX11PixelShader::Is(RenderStage stage) const {
		return stage == RenderStage::RS_PIXEL;
	}

	ComPtr<ID3D11PixelShader> DX11PixelShader::GetD3D11Shader() const {
		return m_d3dShader;
	}
}
