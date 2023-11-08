#include "Engine/Rendering/Engine/RenderPass/GBuffer.h"
#include "Engine/Core/Render/Api/Interface/IContext.h"

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

	Int32 UBuffer::Attach(IBufferResourceData* resource) {
		m_buffers.push_back(resource);
		return static_cast<Int32>(m_buffers.size() - 1);
	}

	const Array<IBufferResourceData*>& UBuffer::GetBuffers() const {
		return m_buffers;
	}

	IBufferResourceData* UBuffer::GetBuffer(Int32 index) const {
		return m_buffers[index];
	}
}