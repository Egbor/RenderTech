#ifndef ISHADER_RESOURCE_H
#define ISHADER_RESOURCE_H

#include "Engine/Core/Render/Api/RenderDef.h"

namespace Engine {
	class IShaderResourceData {
	public:
		virtual bool Is(ShaderType type) const;
	};
}

#endif // !ISHADER_RESOURCE_H
