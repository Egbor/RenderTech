#include "Engine/Object/Class/Skybox.h"

namespace Engine {
	GENERATE_INSTANTIATION(Skybox)

	Skybox::Skybox(const ObjectArgument& arguments) 
		: Super(arguments) {

	}

	Skybox::~Skybox() {
		DELETE_OBJECT(m_envCubemap);
	}

	TextureResource* Skybox::GetNativeResource() const {
		return m_envCubemap->GetNativeResource();
	}

	TextureResource* Skybox::GetIBLNativeResource() const {
		return m_irrCubemap->GetNativeResource();
	}
}