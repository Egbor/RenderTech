#include "Engine/Rendering/Engine/RenderPass/RenderBuffer.h"

namespace Engine {
	template<class TResourceData>
	Int32 ProcessCommonAttachment(Array<ResourceWrapper<TResourceData>>& arr, TResourceData* resource, Int32 slot, bool isNative) {
		arr.push_back(ResourceWrapper<TResourceData>(resource, isNative));
		return static_cast<Int32>(arr.size() - 1);
	}

	GBuffer::GBuffer() 
		: m_targets() {

	}

	Int32 GBuffer::InitNewResource(Int32 slot, ITargetResourceData* resource) {
		return ProcessCommonAttachment(m_targets, resource, slot, false);
	}

	Int32 GBuffer::InitNewResource(Int32 slot, TextureType type, TextureFormat format, Int32 width, Int32 height) {
		IRenderResourceFactory* factory = Core::GetInstance()->GetContext()->QueryResourceFactory();
		return ProcessCommonAttachment(m_targets, factory->CreateTarget(type, format, width, height), slot, true);
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

	const Array<ITextureResourceData*> GBuffer::GetResources(Int32 slot) const {
		Array<ITextureResourceData*> result;
		for (Size i = 0; i < m_targets.size(); i++) {
			if (m_targets[i].IsAssociatedWith(slot)) {
				result.push_back(m_targets[i].GetResource()->GetTextureResource());
			}
		}
		return result;
	}

	UBuffer::UBuffer() 
		: m_buffers() {

	}

	Int32 UBuffer::InitNewResource(Int32 slot, Int32 bufferSize) {
		Array<Int8> dummy(bufferSize);

		IRenderResourceFactory* factory = Core::GetInstance()->GetContext()->QueryResourceFactory();
		return ProcessCommonAttachment(m_buffers, factory->CreateBuffer(BufferType::BT_UNIFORM, 1, dummy.size(), dummy.data()), slot, true);
	}

	void UBuffer::Update(Int32 id) {
		IDynamicResourceData* buffer = dynamic_cast<IDynamicResourceData*>(m_buffers[id].GetResource());
		buffer->Update(Core::GetInstance()->GetContext());
	}

	RawData UBuffer::GetBufferData(Int32 id) const {
		IDynamicResourceData* buffer = dynamic_cast<IDynamicResourceData*>(m_buffers[id].GetResource());
		return buffer->GetBufferData();
	}

	const Array<IBufferResourceData*> UBuffer::GetResources(Int32 slot) const {
		Array<IBufferResourceData*> result;
		for (Size i = 0; i < m_buffers.size(); i++) {
			if (m_buffers[i].IsAssociatedWith(slot)) {
				result.push_back(m_buffers[i].GetResource());
			}
		}
		return result;
	}

	Samplers::Samplers() 
		: m_states() {

	}

	Int32 Samplers::InitNewResource(Int32 slot) {
		IRenderResourceFactory* factory = Core::GetInstance()->GetContext()->QueryResourceFactory();
		return ProcessCommonAttachment(m_states, dynamic_cast<AbstractSamplerState*>(factory->CreateState(StateType::ST_SAMPLER)), slot, true);
	}

	const Array<AbstractSamplerState*> Samplers::GetResources(Int32 slot) const {
		Array<AbstractSamplerState*> result;
		for (Size i = 0; i < m_states.size(); i++) {
			if (m_states[i].IsAssociatedWith(slot)) {
				result.push_back(m_states[i].GetResource());
			}
		}
		return result;
	}
}