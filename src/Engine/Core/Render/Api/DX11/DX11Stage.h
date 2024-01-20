#ifndef DX11STAGE_H
#define DX11STAGE_H

#include "Engine/Core/Render/Api/DX11/DX11Def.h"
#include "Engine/Core/Render/Api/Interface/IRenderStage.h"

namespace Engine {
	class DX11Context;

	class DX11StageVS : public IRenderStage {
	public:
		DX11StageVS(DX11Context* dxContext);
		virtual ~DX11StageVS() = default;

		void BindTextures(const Array<ITextureResourceData*>& resources) override;
		void BindBuffers(const Array<IBufferResourceData*>& resources) override;
		void BindSamplers(const Array<AbstractSamplerState*>& resources) override;
		void BindShader(IShaderResourceData* resource) override;

	private:
		DX11Context* m_dxContext;
	};

	class DX11StagePS : public IRenderStage {
	public:
		DX11StagePS(DX11Context* dxContext);
		virtual ~DX11StagePS() = default;

		void BindTextures(const Array<ITextureResourceData*>& resources) override;
		void BindBuffers(const Array<IBufferResourceData*>& resources) override;
		void BindSamplers(const Array<AbstractSamplerState*>& resources) override;
		void BindShader(IShaderResourceData* resource) override;

	private:
		DX11Context* m_dxContext;
	};
}

#endif // !DX11STAGE_H
