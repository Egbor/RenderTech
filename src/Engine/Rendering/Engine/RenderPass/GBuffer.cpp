#include "Engine/Rendering/Engine/RenderPass/GBuffer.h"

namespace Engine {
	template<class TResourceData>
	Int32 ProcessCommonAttachment(Array<BufferResourceWrapper<TResourceData>>& arr, TResourceData* resource, bool isNative) {
		arr.push_back(BufferResourceWrapper<TResourceData>(resource, isNative));
		return static_cast<Int32>(arr.size() - 1);
	}

	GBuffer::GBuffer() 
		: m_targets() {

	}

	GBuffer::~GBuffer() {
		//DELETE_ARRAY_OF_OBJECTS(m_targets);
	}

	Int32 GBuffer::Attach(ITargetResourceData* resource) {
		return ProcessCommonAttachment(m_targets, resource, false);
	}

	Int32 GBuffer::Attach(TextureType type, TextureFormat format, Int32 width, Int32 height) {
		IRenderResourceFactory* factory = Core::GetInstance()->GetContext()->QueryResourceFactory();
		return ProcessCommonAttachment(m_targets, factory->CreateTarget(type, format, width, height), true);
	}

	void GBuffer::Clear() {
		for (Size i = 0; i < m_targets.size(); i++) {
			m_targets[i].GetResource()->Clear(Core::GetInstance()->GetContext());
		}
	}

	const Array<ITargetResourceData*> GBuffer::GetTargets() const {
		Array<ITargetResourceData*> result(m_targets.size());
		for (Size i = 0; i < result.size(); i++) {
			result[i] = m_targets[i].GetResource();
		}
		return result;
	}

	const Array<ITextureResourceData*> GBuffer::GetTextures() const {
		Array<ITextureResourceData*> result(m_targets.size());
		for (Size i = 0; i < result.size(); i++) {
			result[i] = m_targets[i].GetResource()->GetTextureResource();
		}
		return result;
	}

	UBuffer::UBuffer() 
		: m_buffers() {

	}

	UBuffer::~UBuffer() {
		//DELETE_ARRAY_OF_OBJECTS(m_buffers);
	}


	Int32 UBuffer::Attach(Int32 bufferSize) {
		Array<Int8> dummy(bufferSize);

		IRenderResourceFactory* factory = Core::GetInstance()->GetContext()->QueryResourceFactory();
		return ProcessCommonAttachment(m_buffers, factory->CreateBuffer(BufferType::BT_UNIFORM, 1, dummy.size(), dummy.data()), true);
	}

	void UBuffer::Update(Int32 index) {
		IDynamicResourceData* buffer = dynamic_cast<IDynamicResourceData*>(m_buffers[index].GetResource());
		buffer->Update(Core::GetInstance()->GetContext());
	}

	RawData UBuffer::GetBufferData(Int32 index) const {
		IDynamicResourceData* buffer = dynamic_cast<IDynamicResourceData*>(m_buffers[index].GetResource());
		return buffer->GetBufferData();
	}

	const Array<IBufferResourceData*> UBuffer::GetBuffers() const {
		Array<IBufferResourceData*> result(m_buffers.size());
		for (Size i = 0; i < result.size(); i++) {
			result[i] = m_buffers[i].GetResource();
		}
		return result;
	}
}