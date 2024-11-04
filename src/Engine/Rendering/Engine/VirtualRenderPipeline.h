#ifndef VIRTUAL_RENDER_PIPELINE_H
#define VIRTUAL_RENDER_PIPELINE_H

#include "Engine/Core/Render/Base/Interface/IContext.h"
#include "Engine/Rendering/Engine/RenderBatcher.h"

#include <optional>

namespace Engine {
	struct VirtualRenderStage {
		Array<ITextureResourceData*> m_localTextures;
		Array<IBufferResourceData*> m_localBuffers;
		Array<IStateResourceData*> m_localSamplers;
		IShaderResourceData* m_localShader;
	};

	class VirtualRenderPipeline {
	private:
		IContext* m_apiContext;

	public:
		VirtualRenderPipeline(IContext* apiContext);
		virtual ~VirtualRenderPipeline() = default;

		void DrawIndexedPremitive(IBufferResourceData* vertex, IBufferResourceData* index);
		void DrawIndexedWaveframe(IBufferResourceData* vertex, IBufferResourceData* index);
		void SwapBuffers();

		void SetViewport(Int32 width, Int32 height);

		void ClearTargets(const RenderResourcesStorage& storage, BatchSlot slot);
		void SetDepthStencilCleaningFlags(ITargetResourceData* resource, bool enableDepthCleaning, bool enableStencilCleaning, Int32 stencilValue = 0);
		void UpdateBuffer(IBufferResourceData* resource, std::function<void(RawData&)> updater);

		Int32 GetRenderSpaceWidth() const;
		Int32 GetRenderSpaceHeight() const;

		ITargetResourceData* GetTarget() const;

		template<class TResourceClass>
		void AddResource(RenderStage stage, TResourceClass* resource);

		template<class TResourceClass>
		void AddResource(TResourceClass* resource);

		void AddResourcesFromTargetsToTextures(const RenderResourcesStorage& storage, BatchSlot slot, RenderStage stage);

		template<class TResourceClass>
		void AddResources(const RenderResourcesStorage& storage, BatchSlot slot, RenderStage stage);
		
		template<class TResourceClass>
		void AddResources(const RenderResourcesStorage& storage, BatchSlot slot);

		template<class TResourceClass>
		void AddResources(RenderStage stage, const Array<TResourceClass*>& resources);

		template<class TResourceClass>
		void AddResources(const Array<TResourceClass*>& resources);

	private:
		inline void AdjustViewport();
		void SetApiPipeline();

		Viewport m_localViewport;
		Array<ITargetResourceData*> m_localGBuffer;
		Array<IStateResourceData*> m_localStates;
		Array<VirtualRenderStage> m_localStages;
	};

	class HighRenderPipeline {

	};
}

#endif // !VIRTUAL_RENDER_PIPELINE_H
