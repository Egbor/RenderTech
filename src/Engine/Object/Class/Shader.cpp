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

	Shader::Metadata::Metadata() 
		: m_data(nullptr) {

	}

	Shader::Metadata::~Metadata() {
		if (m_data != nullptr) {
			delete[] m_data;
			m_data = nullptr;
		}
	}


	Shader::Metadata* Shader::Metadata::SetType(RenderStage stage) {
		m_stage = stage;
		return this;
	}

	Shader::Metadata* Shader::Metadata::SetDataLength(Int32 length) {
		if (m_data != nullptr) {
			delete[] m_data;
			m_data = nullptr;
		}

		m_dataLength = length;
		m_data = new Int8[length];

		return this;
	}

	Int8* Shader::Metadata::GetData() const {
		return m_data;
	}

	Object* Shader::Metadata::Build() {
		IRenderResourceFactory* factory = Core::GetInstance()->GetContext()->QueryResourceFactory();

		Shader* shader = ClassType<Shader>::CreateObject(ObjectArgument::Dummy());
		shader->m_stage = m_stage;
		shader->m_shader = factory->CreateShader(m_stage, m_dataLength, m_data);

		return shader;
	}

	void Shader::InitializeNativeResource(RenderStage stage, const Array<Int8>& code) {
		IRenderResourceFactory* factory = Core::GetInstance()->GetContext()->QueryResourceFactory();
		m_shader = factory->CreateShader(stage, code.size(), code.data());
		m_stage = stage;
	}
}