#ifndef HIGH_RENDER_CONTEXT_H
#define HIGH_RENDER_CONTEXT_H

#include "Engine/Rendering/Engine/VirtualRenderPipeline.h"
#include "Engine/Rendering/Engine/HighRenderCommand.h"
#include "Engine/Rendering/Engine/Scene/Scene.h"

#include "Engine/Object/Class/Texture.h"

namespace Engine {
	class AbstractHighRenderContext {
	private:
		VirtualRenderPipeline* m_pipeline;
		Array<IHighRenderCommand*> m_commands;

	public:
		AbstractHighRenderContext(IContext* context);
		virtual ~AbstractHighRenderContext();

		void ExtendCommandList(IHighRenderCommand* command);
		void DrawScene(Scene* scene);

		virtual void DrawInit(IRenderResourceFactory* factory) = 0;

	protected:
		VirtualRenderPipeline* GetPipeline() const;
		const RenderResourcesStorage& GetStorage() const;

		virtual void OnPreDraw() = 0;
		virtual void OnPostDraw() = 0;

		template<class TResourceData>
		inline TResourceData* GetResourceFromBatchByTag(const String& tag) const {
			return m_storage.GetResourceFromBatchByTag<TResourceData>(tag);
		}

		inline void InitResourceForBatchOfStates(IRenderResourceFactory* factory, EnumFlags<BatchSlot> slots, const String& tag, StateType type, StateData data);
		inline void InitResourceForBatchOfBuffers(IRenderResourceFactory* factory, EnumFlags<BatchSlot> slots, const String& tag, Int32 bufferSize);
		inline void InitResourceForBatchOfTargets(IRenderResourceFactory* factory, EnumFlags<BatchSlot> slots, const String& tag, TextureType type, TextureFormat format, Int32 width, Int32 height);
		inline void InitResourceForBatchOfTargets(ITargetResourceData* resource, EnumFlags<BatchSlot> slots, const String& tag);

	private:
		RenderResourcesStorage m_storage;
	};

	class HRC_Base : public AbstractHighRenderContext {
	public:
		HRC_Base(IContext* context);
		virtual ~HRC_Base() = default;

		void DrawInit(IRenderResourceFactory* factory) override;

	private:
		void OnPreDraw() override;
		void OnPostDraw() override;
	};

	class HRC_IBLBacker : public AbstractHighRenderContext {
	private:
		Int32 m_IBLCubeMapOutputWidth;
		Int32 m_IBLCubeMapOutputHeight;

		Texture2D* m_texture2D;

	public:
		HRC_IBLBacker(IContext* context, const String& filename, Int32 outputWidth, Int32 outputHeight);
		virtual ~HRC_IBLBacker();

		void DrawInit(IRenderResourceFactory* factory) override;

	private:
		void OnPreDraw() override;
		void OnPostDraw() override;
	};
}

#endif // !HIGH_RENDER_CONTEXT_H
