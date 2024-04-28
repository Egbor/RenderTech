#include "Engine/Object/Class/Skybox.h"
#include "Engine/Core/System/Resource/Resource.h"

namespace Engine {
	GENERATE_INSTANTIATION(Skybox)

	Skybox::Skybox(const ObjectArgument& arguments) 
		: Super(arguments) {
		m_envCubemap = Resource::Load<TextureCube*>("assets/textures/skybox/afternoon_env.exr");
	}

	Skybox::~Skybox() {
		DELETE_OBJECT(m_envCubemap);
	}

	ITextureResourceData* Skybox::GetNativeResource() const {
		return m_envCubemap->GetNativeResource();
	}
}