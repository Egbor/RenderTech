#ifndef DX11STAGE_H
#define DX11STAGE_H

#include "Engine/Core/Render/Api/DX11/DX11Def.h"
#include "Engine/Core/Render/Api/Interface/IRenderStage.h"

namespace Engine {
	class DX11StageVS : public IRenderStage {
	public:
		DX11StageVS(ComPtr<ID3D11Device> d3dDevice, ComPtr<ID3D11DeviceContext> d3dContext);
		virtual ~DX11StageVS() = default;

		void BindTextures(const Array<ITextureResourceData*>& resources) override;
		void BindBuffers(const Array<IBufferResourceData*>& resources) override;
		void BindShader(IShaderResourceData* resource) override;

	private:
		ComPtr<ID3D11Device> m_d3dDevice;
		ComPtr<ID3D11DeviceContext> m_d3dContext;
	};

	class DX11StagePS : public IRenderStage {
	public:
		DX11StagePS(ComPtr<ID3D11Device> d3dDevice, ComPtr<ID3D11DeviceContext> d3dContext);
		virtual ~DX11StagePS() = default;

		void BindTextures(const Array<ITextureResourceData*>& resources) override;
		void BindBuffers(const Array<IBufferResourceData*>& resources) override;
		void BindShader(IShaderResourceData* resource) override;

	private:
		ComPtr<ID3D11Device> m_d3dDevice;
		ComPtr<ID3D11DeviceContext> m_d3dContext;
	};
}

#endif // !DX11STAGE_H
