#include "Engine/Object/Class/Shader.h"
#include "Engine/Core/Core.h"

namespace Engine {
	GENERATE_INSTANTIATION(Shader)

	Shader::Shader(const ObjectArgument& arguments) 
		: Object(arguments), m_shader(nullptr), m_stage() {

	}

	Shader::~Shader() {
		DELETE_OBJECT(m_shader)
	}

	RenderStage Shader::GetAssociatedStage() const {
		return m_stage;
	}

	ShaderResource* Shader::GetNativeResource() const {
		return m_shader;
	}

	void Shader::InitializeNativeResource(RenderStage stage, const Array<Int8>& code) {
		IRenderResourceFactory* factory = Core::GetInstance()->GetContext()->QueryResourceFactory();
		m_shader = factory->CreateShader(stage, code.size(), code.data());
		m_stage = stage;
	}
}