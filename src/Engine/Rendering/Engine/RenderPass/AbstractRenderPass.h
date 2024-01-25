#ifndef ABSTRACT_RENDER_PASS_H
#define ABSTRACT_RENDER_PASS_H

#include "Engine/Core/Render/Api/Interface/IContext.h"
#include "Engine/Rendering/Engine/RenderPass/RenderBuffer.h"

#include "Engine/Object/Class/Mesh.h"

namespace Engine {
	enum class RenderPassType {
		RP_BASE,
		RP_LIGHT
	};

	struct RenderModel {
		Matrix4x4 world;
		Mesh* mesh;
	};

	struct RenderLight {
		Vector4 value;
		Vector4 color;
		Float brightness;
		RenderModel space;
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

		GBuffer& GetGBuffer();
		UBuffer& GetUBuffer();
		Samplers& GetSamplers();

	protected:
		static IShaderResourceData* LoadShader(const String& filename, ShaderType type);

	private:
		GBuffer m_gbuffer;
		UBuffer m_ubuffer;
		Samplers m_samplers;
	};
}

#endif // !ABSTRACT_RENDER_PASS_H
