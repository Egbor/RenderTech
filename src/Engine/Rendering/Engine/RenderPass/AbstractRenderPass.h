#ifndef ABSTRACT_RENDER_PASS_H
#define ABSTRACT_RENDER_PASS_H

#include "Engine/Core/Render/Api/Interface/IContext.h"
#include "Engine/Rendering/Engine/RenderPass/GBuffer.h"

namespace Engine {
	enum class RenderPassType {
		RP_BASE,
		RP_LIGHT
	};

	class AbstractRenderPass {
	public:
		AbstractRenderPass() = default;
		virtual ~AbstractRenderPass() = default;

		Float GetRenderWidth() const;
		Float GetRenderHeight() const;

		virtual void Initialize(ITargetResourceData* output) = 0;
		virtual void Launch(IRenderPipeline* pipeline, AbstractRenderPass* prev = nullptr) = 0;
		virtual bool Is(RenderPassType type) const = 0;

	protected:
		GBuffer& GetGBuffer();
		UBuffer& GetUBuffer();

		static IShaderResourceData* LoadShader(const String& filename, ShaderType type);

	private:
		GBuffer m_gbuffer;
		UBuffer m_ubuffer;
	};
}

#endif // !ABSTRACT_RENDER_PASS_H
