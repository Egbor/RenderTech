#include "Engine/Rendering/RenderSet/SceneRenderSet.h"

#include "Engine/Rendering/RenderPass/MeshRenderPass.h"
#include "Engine/Rendering/RenderPass/LightRenderPass.h"
#include "Engine/Rendering/RenderPass/SkyboxRenderPass.h"

namespace Engine {
	SceneRenderSet::SceneRenderSet(Context* context, Texture2D* target) 
		: m_context(context), m_target(target) {
		AddRenderPass<MeshRenderPass, Context*, Texture2D*>(context, target);
		AddRenderPass<LightRenderPass, Context*, Texture2D*>(context, target);
		AddRenderPass<SkyboxRenderPass, Context*, Texture2D*>(context, target);
	}

	void SceneRenderSet::PreExecute() {
		m_context->SetViewport(m_target->GetWidth(), m_target->GetHeight());
	}
}