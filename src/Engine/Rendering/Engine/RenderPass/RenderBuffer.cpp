#include "Engine/Rendering/Engine/RenderPass/RenderBuffer.h"
#include "Engine/Core/System/Exception/EngineException.h"

namespace Engine {
	template<class TResourceData>
	Int32 ProcessCommonInit(Array<ResourceSlot<TResourceData>>& arr, TResourceData* resource, EnumFlags<BatchSlot> batchIds) {
		arr.push_back(ResourceSlot<TResourceData>(batchIds, resource));
		return static_cast<Int32>(arr.size() - 1);
	}

	template<class TResourceData, class TOutResourceData>
	const Array<TOutResourceData*> ProcessCommonGet(BatchSlot batchId, const Array<ResourceSlot<TResourceData>>& arr, std::function<TOutResourceData*(TResourceData*)> callback) {
		Array<TOutResourceData*> result;
		for (Size i = 0; i < arr.size(); i++) {
			if (arr[i].IsAssociatedWith(batchId)) {
				result.push_back(callback(arr[i].GetResource()));
			}
		}
		return result;
	}

	GBuffer::GBuffer() 
		: m_batch() {

	}

	void GBuffer::Bind(BatchSlot batchId, IRenderStage* stage) {
		const Array<ITextureResourceData*> textures = ProcessCommonGet<ITargetResourceData, ITextureResourceData>(batchId, m_batch, [](ITargetResourceData* resource) { return resource->GetTextureResource(); });
		stage->BindTextures(textures);
	}

	void GBuffer::Bind(BatchSlot batchId, IRenderPipeline* pipeline) {
		const Array<ITargetResourceData*> targets = ProcessCommonGet<ITargetResourceData, ITargetResourceData>(batchId, m_batch, [](ITargetResourceData* resource) { return resource; });
		pipeline->SetTargets(targets);
	}

	void GBuffer::InitNewResource(EnumFlags<BatchSlot> batchIds, ITargetResourceData* resource) {
		ProcessCommonInit(m_batch, resource, batchIds);
	}

	void GBuffer::InitNewResource(EnumFlags<BatchSlot> batchIds, TextureType type, TextureFormat format, Int32 width, Int32 height) {
		IRenderResourceFactory* factory = Core::GetInstance()->GetContext()->QueryResourceFactory();
		ProcessCommonInit(m_batch, factory->CreateTarget(type, format, width, height), batchIds | BatchSlot::BS_SLOT_DELETABLE);
	}

	void GBuffer::Clear() {
		for (Size i = 0; i < m_batch.size(); i++) {
			m_batch[i].GetResource()->Clear(Core::GetInstance()->GetContext());
		}
	}

	UBuffer::UBuffer() 
		: m_batch() {

	}

	void UBuffer::Bind(BatchSlot batchId, IRenderStage* stage) {
		const Array<IBufferResourceData*> buffers = ProcessCommonGet<IBufferResourceData, IBufferResourceData>(batchId, m_batch, [](IBufferResourceData* resource) { return resource; });
		stage->BindBuffers(buffers);
	}

	void UBuffer::InitNewResource(EnumFlags<BatchSlot> batchIds, Int32 bufferSize, Int32* outId) {
		Array<Int8> dummy(bufferSize);

		IRenderResourceFactory* factory = Core::GetInstance()->GetContext()->QueryResourceFactory();
		*outId = ProcessCommonInit(m_batch, factory->CreateBuffer(BufferType::BT_UNIFORM, 1, dummy.size(), dummy.data()), batchIds | BatchSlot::BS_SLOT_DELETABLE);
	}

	void UBuffer::Update(Int32 id) {
		IDynamicResourceData* buffer = dynamic_cast<IDynamicResourceData*>(m_batch[id].GetResource());
		buffer->Update(Core::GetInstance()->GetContext());
	}

	RawData UBuffer::GetBufferData(Int32 id) const {
		IDynamicResourceData* buffer = dynamic_cast<IDynamicResourceData*>(m_batch[id].GetResource());
		return buffer->GetBufferData();
	}

	Samplers::Samplers() 
		: m_batch() {

	}

	void Samplers::Bind(BatchSlot batchId, IRenderStage* stage) {
		const Array<IStateResourceData*> states = ProcessCommonGet<IStateResourceData, IStateResourceData>(batchId, m_batch, [](IStateResourceData* resource) { return resource; });
		stage->BindSamplers(states);
	}

	void Samplers::InitNewResource(EnumFlags<BatchSlot> batchIds) {
		IRenderResourceFactory* factory = Core::GetInstance()->GetContext()->QueryResourceFactory();
		ProcessCommonInit(m_batch, factory->CreateState(StateType::ST_SAMPLER), batchIds | BatchSlot::BS_SLOT_DELETABLE);
	}
}