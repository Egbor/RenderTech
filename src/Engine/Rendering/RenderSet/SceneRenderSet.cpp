#include "Engine/Rendering/RenderSet/SceneRenderSet.h"

#include "Engine/Rendering/RenderPass/MeshRenderPass.h"
#include "Engine/Rendering/RenderPass/LightRenderPass.h"
#include "Engine/Rendering/RenderPass/SkyboxRenderPass.h"

namespace Engine {
	SceneRenderSet::SceneRenderSet(Context* context, Texture2D* target)
		: RenderSet(target),  m_context(context), 
		m_viewportWidth(target->GetWidth()), m_viewportHeight(target->GetHeight()) {
		AddRenderPass<MeshRenderPass, Context*, Texture2D*>(context, target);
		AddRenderPass<LightRenderPass, Context*, Texture2D*>(context, target);
		AddRenderPass<SkyboxRenderPass, Context*, Texture2D*>(context, target);
	}

	void SceneRenderSet::SetViewport(Int32 width, Int32 height) {
		m_viewportWidth = width;
		m_viewportHeight = height;
	}

	void SceneRenderSet::PreExecute() {
		m_context->SetViewport(m_viewportWidth, m_viewportHeight);
	}
}