#ifndef DX11STAGE_H
#define DX11STAGE_H

#include "Engine/Core/Render/Api/DX11/DX11Context.h"
#include "Engine/Core/Render/Base/Interface/IRenderStage.h"

namespace Engine {
	class DX11StageVS : public IRenderStage {
	public:
		DX11StageVS(DX11Context* context);
		virtual ~DX11StageVS() = default;

		void BindTextures(const Array<TextureResource*>& resources) override;
		void BindBuffers(const Array<BufferResource*>& resources) override;
		void BindSamplers(const Array<StateResource*>& resources) override;
		void BindShader(ShaderResource* resource) override;

		void Disable() override;
		void Enable() override;

	private:
		Int32 m_countOfTakenTexturesSlots;
		bool m_isEnable;

		DX11Context* m_dxContext;
		ShaderResource* m_lastBindedShader;
	};

	class DX11StagePS : public IRenderStage {
	public:
		DX11StagePS(DX11Context* dxContext);
		virtual ~DX11StagePS() = default;

		void BindTextures(const Array<TextureResource*>& resources) override;
		void BindBuffers(const Array<BufferResource*>& resources) override;
		void BindSamplers(const Array<StateResource*>& resources) override;
		void BindShader(ShaderResource* resource) override;

		void Disable() override;
		void Enable() override;

	private:
		Int32 m_countOfTakenTexturesSlots;
		bool m_isEnable;

		DX11Context* m_dxContext;
		ShaderResource* m_lastBindedShader;
	};
}

#endif // !DX11STAGE_H
