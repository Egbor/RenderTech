#ifndef SHADER_RESOURCE_H
#define SHADER_RESOURCE_H

#include "Engine/Core/Render/Base/RenderBase.h"

namespace Engine {
	class ShaderResource : public RenderBase {
	public:
		ShaderResource(IContext* context) : RenderBase(ResourceIdentifier::RI_SHADER, context) {}
		virtual ~ShaderResource() = default;

		virtual bool Is(RenderStage stage) const = 0;

		static ResourceIdentifier GetResourceIdentifier() { return ResourceIdentifier::RI_SHADER; }
	};
}

#endif // !SHADER_RESOURCE_H
