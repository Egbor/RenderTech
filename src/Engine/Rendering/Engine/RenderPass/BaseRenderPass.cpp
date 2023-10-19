#include "Engine/Rendering/Engine/RenderPass/BaseRenderPass.h"

namespace Engine {
	enum class GBuffer_BaseRenderPass {
		GB_BaseColor,
		GB_ViewNormal,
		GB_ORM,
		GB_DEPTH,
		GB_NUM
	};

	BaseRenderPass::BaseRenderPass(IContext* context) 
		: m_gbuffer(static_cast<Size>(GBuffer_BaseRenderPass::GB_NUM)) {
		IRenderResourceFactory* factory = context->QueryResourceFactory();
		m_gbuffer[static_cast<Size>(GBuffer_BaseRenderPass::GB_BaseColor)] = factory->CreateTexture(TextureType::TT_DEFAULT, TextureFormat::TF_R8G8B8A8_BMP, context->GetWidth(), context->GetHeight());
		m_gbuffer[static_cast<Size>(GBuffer_BaseRenderPass::GB_ViewNormal)] = factory->CreateTexture(TextureType::TT_DEFAULT, TextureFormat::TF_R32G32B32A32_FLOAT, context->GetWidth(), context->GetHeight());
		m_gbuffer[static_cast<Size>(GBuffer_BaseRenderPass::GB_ORM)] = factory->CreateTexture(TextureType::TT_DEFAULT, TextureFormat::TF_B8G8R8A8_BMP, context->GetWidth(), context->GetHeight());
		m_gbuffer[static_cast<Size>(GBuffer_BaseRenderPass::GB_DEPTH)] = factory->CreateTexture(TextureType::TT_DEPTH, TextureFormat::TF_R24_BMP_G8_UINT, context->GetWidth(), context->GetHeight());
	}

	BaseRenderPass::~BaseRenderPass() {
		DELETE_ARRAY_OF_OBJECTS(m_gbuffer);
	}

	void BaseRenderPass::Launch(IRenderPipeline* pipeline) {
		pipeline->SetGBuffer(m_gbuffer);
	}

	bool BaseRenderPass::Is(RenderPassType type) const {
		return type == RenderPassType::RP_BASE;
	}
}