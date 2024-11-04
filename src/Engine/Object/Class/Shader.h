#ifndef SHADER_H
#define SHADER_H

#include "Engine/Object/Object.h"
#include "Engine/Core/Render/Base/Resource/ShaderResource.h"

namespace Engine {
	CLASSTYPE(Shader)
	class Shader : public Object {
		GENERATE_BODY(Shader, Object)

	private:
		ShaderResource* m_shader;
		RenderStage m_stage;

	public:
		Shader(const ObjectArgument& arguments);
		virtual ~Shader();

		RenderStage GetAssociatedStage() const;
		ShaderResource* GetNativeResource() const;

		void Init(RenderStage stage);
		void Build(const Array<Int8>& code);
	};
}

#endif // !SHADER_H
