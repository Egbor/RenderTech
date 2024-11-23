#include "Engine/Object/Class/Shader.h"
#include "Engine/Core/Core.h"

namespace Engine {
	GENERATE_INSTANTIATION(Shader)

	Shader::Shader(const ObjectArgument& arguments) 
		: Object(arguments), m_shader(nullptr), m_stage(), m_name() {

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

	Shader::Metadata* Shader::Metadata::SetName(const String& value) {
		return dynamic_cast<Shader::Metadata*>(_SetName(value));
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

	IResourceMetadata* Shader::Metadata::_SetName(const String& value) {
		m_name = value;
		return this;
	}

	Object* Shader::Metadata::Build() {
		assert(!m_name.empty());

		IRenderResourceFactory* factory = Core::GetInstance()->GetContext()->QueryResourceFactory();

		Shader* shader = ClassType<Shader>::CreateObject(ObjectArgument::Dummy());
		shader->m_stage = m_stage;
		shader->m_shader = factory->CreateShader(m_stage, m_name + "_shdrRes", m_dataLength, m_data);

		shader->SetName(m_name);
		return shader;
	}
}