#include "Engine/Rendering/Engine/RenderPass/GBuffer.h"
#include "Engine/Core/Render/Api/Interface/IContext.h"
#include "Engine/Core/Core.h"

namespace Engine {
	GBuffer::GBuffer() 
		: m_targets() {

	}

	GBuffer::~GBuffer() {
		DELETE_ARRAY_OF_OBJECTS(m_targets);
	}

	Int32 GBuffer::Attach(ITargetResourceData* resource) {
		m_targets.push_back(resource);
		return static_cast<Int32>(m_targets.size() - 1);
	}

	Int32 GBuffer::Attach(TextureType type, TextureFormat format, Int32 width, Int32 height) {
		IRenderResourceFactory* factory = Core::GetInstance()->GetContext()->QueryResourceFactory();
		return this->Attach(factory->CreateTarget(TextureType::TT_DEFAULT, TextureFormat::TF_R8G8B8A8_BMP, width, height));
	}

	void GBuffer::Clear(IRenderPipeline* context) {
		for (Size i = 0; i < m_targets.size(); i++) {
			m_targets[i]->Clear(dynamic_cast<IContext*>(context));
		}
	}

	const Array<ITargetResourceData*>& GBuffer::GetTargets() const {
		return m_targets;
	}

	const Array<ITextureResourceData*> GBuffer::GetTextures() const {
		Array<ITextureResourceData*> result(m_targets.size());
		for (Size i = 0; i < result.size(); i++) {
			result[i] = m_targets[i]->GetTextureResource();
		}
		return result;
	}

	UBuffer::UBuffer() 
		: m_buffers() {

	}

	UBuffer::~UBuffer() {
		DELETE_ARRAY_OF_OBJECTS(m_buffers);
	}

	const Array<IBufferResourceData*>& UBuffer::GetBuffers() const {
		return m_buffers;
	}

	IBufferResourceData* UBuffer::GetBuffer(Int32 index) const {
		return m_buffers[index];
	}
}