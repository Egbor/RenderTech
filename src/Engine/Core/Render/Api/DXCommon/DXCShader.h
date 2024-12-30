#ifndef DXCSHADER_H
#define DXCSHADER_H

#include "Engine/Core/Render/Api/DXCommon/DXCDef.h"
#include "Engine/Core/Render/Base/Resource/ShaderResource.h"

namespace Engine {
	class DXCShader : public ShaderResource {
	public:
		DXCShader(const String& name, IContext* context, Size codeLength, const void* code) 
			: ShaderResource(name, context) {}

		ComPtr<ID3DBlob> GetD3DInterface() const { return nullptr; }

	private:
		ComPtr<ID3DBlob> m_d3dShader;
	};
}

#endif // !DXCSHADER_H
