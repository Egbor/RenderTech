#ifndef SKYBOX_H
#define SKYBOX_H

#include "Engine/Object/Object.h"
#include "Engine/Object/Class/Texture.h"

namespace Engine {
	CLASSTYPE(Skybox)
	class Skybox : public Object {
		GENERATE_BODY(Skybox, Object)

	private:
		TextureCube* m_envCubemap;

	public:
		Skybox(const ObjectArgument& arguments);
		virtual ~Skybox();

		ITextureResourceData* GetNativeResource() const;
	};
}

#endif // !SKYBOX_H
