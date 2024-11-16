#ifndef SHADER_H
#define SHADER_H

#include "Engine/Core/Render/Base/Resource/ShaderResource.h"
#include "Engine/Core/System/Resource/ResourceMetadata.h"
#include "Engine/Object/Object.h"

namespace Engine {
	CLASSTYPE(Shader)
	class Shader : public Object {
		GENERATE_BODY(Shader, Object)

	private:
		ShaderResource* m_shader;
		RenderStage m_stage;

	public:
		class Metadata : public IResourceMetadata {
		public:
			Metadata();
			~Metadata();

			Metadata* SetType(RenderStage stage);
			Metadata* SetDataLength(Int32 length);
			Int8* GetData() const;

			Object* Build() override;

		private:
			Int8* m_data;
			Int32 m_dataLength;
			RenderStage m_stage;
		};

		Shader(const ObjectArgument& arguments);
		virtual ~Shader();

		RenderStage GetAssociatedStage() const;
		ShaderResource* GetNativeResource() const;

		void Init(RenderStage stage);
		void Build(const Array<Int8>& code);
	};
}

#endif // !SHADER_H
