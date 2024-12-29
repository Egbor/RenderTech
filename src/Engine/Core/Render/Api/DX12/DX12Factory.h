#ifndef DX12FACTORY_H
#define DX12FACTORY_H

#include "Engine/Core/Render/Api/DX12/DX12Def.h"
#include "Engine/Core/Render/Base/Interface/IRenderResourceFactory.h"

namespace Engine {
	class DX12Factory : public IRenderResourceFactory {
	public:
		DX12Factory(ComPtr<ID3D12Device> d3dDevice);

		StateResource* CreateState(StateType type, const String& name, StateData data) override;
		ShaderResource* CreateShader(RenderStage stage, const String& name, Size codeLength, const void* code) override;
		BufferResource* CreateBuffer(BufferType type, const String& name, Int32 size, Int32 strides, const void* data) override;
		TargetResource* CreateTarget(TextureType type, const String& name, TextureFormat format, Int32 width, Int32 height) override;
		TextureResource* CreateTexture(TextureType type, const String& name, TextureFormat format, Int32 width, Int32 height, Array<Int8*> data) override;

	private:
		ComPtr<ID3D12DescriptorHeap> m_d3dRTVDescriptionHeap;
	};
}

#endif // !DX12FACTORY_H
