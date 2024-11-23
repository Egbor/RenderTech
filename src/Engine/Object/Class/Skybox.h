#ifndef SKYBOX_H
#define SKYBOX_H

#include "Engine/Object/Object.h"
#include "Engine/Object/Class/Texture.h"

namespace Engine {
	CLASSTYPE(Skybox)
	class Skybox : public Object {
		GENERATE_BODY(Skybox, Object)

	private:
		Texture2D* m_envCubemap;
		Texture2D* m_irrCubemap;

	public:
		Skybox(const ObjectArgument& arguments);
		virtual ~Skybox();

		TextureResource* GetNativeResource() const;
		TextureResource* GetIBLNativeResource() const;
	};
}

#endif // !SKYBOX_H
