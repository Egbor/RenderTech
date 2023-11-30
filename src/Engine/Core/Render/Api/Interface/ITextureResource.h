#ifndef ITEXTURE_H
#define ITEXTURE_H

#include "Engine/Core/Render/Api/RenderDef.h"

namespace Engine {
	class ITextureResourceData {
	public:
		virtual ~ITextureResourceData() = default;
		virtual Int32 GetWidth() const = 0;
		virtual Int32 GetHeight() const = 0;
		virtual TextureFormat GetFormat() const = 0;

		virtual bool IsCubemap() const = 0;
	};
}

#endif // !ITEXTURE_H
