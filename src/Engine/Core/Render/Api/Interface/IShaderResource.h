#ifndef ISHADER_RESOURCE_H
#define ISHADER_RESOURCE_H

#include "Engine/Core/Render/Api/RenderDef.h"

namespace Engine {
	class IShaderResourceData {
	public:
		virtual ~IShaderResourceData() = default;
		virtual bool Is(ShaderType type) const = 0;
	};
}

#endif // !ISHADER_RESOURCE_H
