#ifndef DX11STAGE_H
#define DX11STAGE_H

#include "Engine/Core/Render/Api/DX11/DX11Context.h"
#include "Engine/Core/Render/Api/Interface/IRenderStage.h"

namespace Engine {
	class DX11AbstractStage : public IRenderStage {
	public:
		DX11AbstractStage(DX11Context* context);
		virtual ~DX11AbstractStage() = default;

	protected:
		void BindTexturesWithCallback(const Array<ITextureResourceData*>& resources, std::function<void(ComPtr<ID3D11DeviceContext>, const Array<ID3D11ShaderResourceView*>&)> bindCallback);
		void BindBuffersWithCallback(const Array<IBufferResourceData*>& resources, std::function<void(ComPtr<ID3D11DeviceContext>, const Array<ID3D11Buffer*>&)> bindCallback);

		DX11Context* m_context;
	};

	class DX11StageVS : public DX11AbstractStage {
	public:
		DX11StageVS(DX11Context* context);
		virtual ~DX11StageVS() = default;

		void BindTextures(const Array<ITextureResourceData*>& resources) override;
		void BindBuffers(const Array<IBufferResourceData*>& resources) override;
		void BindShader(IShaderResourceData* resource) override;
	};

	class DX11StagePS : public DX11AbstractStage {
	public:
		DX11StagePS(DX11Context* d3dContext);
		virtual ~DX11StagePS() = default;

		void BindTextures(const Array<ITextureResourceData*>& resources) override;
		void BindBuffers(const Array<IBufferResourceData*>& resources) override;
		void BindShader(IShaderResourceData* resource) override;
	};
}

#endif // !DX11STAGE_H
