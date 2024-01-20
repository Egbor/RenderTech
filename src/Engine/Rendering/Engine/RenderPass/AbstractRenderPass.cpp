#include "Engine/Rendering/Engine/RenderPass/AbstractRenderPass.h"
#include "Engine/Core/System/Resource/Resource.h"

namespace Engine {
	Float AbstractRenderPass::GetRenderWidth() const {
		Viewport viewport;
		Core::GetInstance()->GetContext()->QueryPipeline()->GetViewport(viewport);
		return viewport.width;
	}

	Float AbstractRenderPass::GetRenderHeight() const {
		Viewport viewport;
		Core::GetInstance()->GetContext()->QueryPipeline()->GetViewport(viewport);
		return viewport.height;
	}

	GBuffer& AbstractRenderPass::GetGBuffer() {
		return m_gbuffer;
	}

	UBuffer& AbstractRenderPass::GetUBuffer() {
		return m_ubuffer;
	}

	Samplers& AbstractRenderPass::GetSamplers() {
		return m_samplers;
	}

	IShaderResourceData* AbstractRenderPass::LoadShader(const String& filename, ShaderType type) {
		const Array<Int8> code = Resource::Load<const Array<Int8>>(filename);

		IRenderResourceFactory* factory = Core::GetInstance()->GetContext()->QueryResourceFactory();
		return factory->CreateShader(type, code.size(), code.data());
	}
}