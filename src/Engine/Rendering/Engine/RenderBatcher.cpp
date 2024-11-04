#include "Engine/Rendering/Engine/RenderBatcher.h"
#include "Engine/Core/System/Exception/EngineException.h"

namespace Engine {
	//template<>
	//IStateResourceData* RenderResourcesStorage::GetResourceFromBatchByTag(const String& tag) const {
	//	return m_batchOfStates.Get(m_resourcesIds.at(tag));
	//}

	//template<>
	//IBufferResourceData* RenderResourcesStorage::GetResourceFromBatchByTag(const String& tag) const {
	//	return m_batchOfBuffers.Get(m_resourcesIds.at(tag));
	//}

	//template<>
	//ITargetResourceData* RenderResourcesStorage::GetResourceFromBatchByTag(const String& tag) const {
	//	return m_batchOfTargets.Get(m_resourcesIds.at(tag));
	//}

	//template<>
	//ITextureResourceData* RenderResourcesStorage::GetResourceFromBatchByTag(const String& tag) const {
	//	return m_batchOfTextures.Get(m_resourcesIds.at(tag));
	//}

	//template<>
	//void RenderResourcesStorage::HandleResources(BatchSlot slot, std::function<void(IStateResourceData*)> callback) const {
	//	m_batchOfStates.HandleResources(slot, callback);
	//}

	//template<>
	//void RenderResourcesStorage::HandleResources(BatchSlot slot, std::function<void(IBufferResourceData*)> callback) const {
	//	m_batchOfBuffers.HandleResources(slot, callback);
	//}

	//template<>
	//void RenderResourcesStorage::HandleResources(BatchSlot slot, std::function<void(ITargetResourceData*)> callback) const {
	//	m_batchOfTargets.HandleResources(slot, callback);
	//}

	//template<>
	//void RenderResourcesStorage::HandleResources(BatchSlot slot, std::function<void(ITextureResourceData*)> callback) const {
	//	m_batchOfTextures.HandleResources(slot, callback);
	//}

	//void RenderResourcesStorage::InitResourceForBatchOfStates(IRenderResourceFactory* factory, EnumFlags<BatchSlot> slots, const String& tag, StateType type, StateData data) {
	//	m_resourcesIds[tag] = m_batchOfStates.Insert(slots | BatchSlot::BS_DELETABLE, factory->CreateState(type, data));
	//}

	//void RenderResourcesStorage::InitResourceForBatchOfBuffers(IRenderResourceFactory* factory, EnumFlags<BatchSlot> slots, const String& tag, Int32 bufferSize) {
	//	Array<Int8> dummy(bufferSize);
	//	m_resourcesIds[tag] = m_batchOfBuffers.Insert(slots | BatchSlot::BS_DELETABLE, factory->CreateBuffer(BufferType::BT_UNIFORM, 1, static_cast<Int32>(dummy.size()), dummy.data()));
	//}

	//void RenderResourcesStorage::InitResourceForBatchOfTargets(IRenderResourceFactory* factory, EnumFlags<BatchSlot> slots, const String& tag, TextureType type, TextureFormat format, Int32 width, Int32 height) {
	//	m_resourcesIds[tag] = m_batchOfTargets.Insert(slots | BatchSlot::BS_DELETABLE, factory->CreateTarget(type, format, width, height));
	//}

	//void RenderResourcesStorage::InitResourceForBatchOfTargets(ITargetResourceData* resource, EnumFlags<BatchSlot> slots, const String& tag) {
	//	m_resourcesIds[tag] = m_batchOfTargets.Insert(slots, resource);
	//}

	HighRenderStorage::~HighRenderStorage() {
		for (Size i = 0; i < m_storage.size(); i++) {
			if (m_storage[i].isDeletable) {
				DELETE_OBJECT(m_storage[i].data);
			}
		}
	}

	void HighRenderStorage::InitResourceAsState(IRenderResourceFactory* factory, const String& name, StateType type, StateData data) {
		m_storage.push_back({ name, dynamic_cast<RenderBase*>(factory->CreateState(type, data)), {}, true });
	}

	void HighRenderStorage::InitResourceAsBuffer(IRenderResourceFactory* factory, const String& name, Int32 bufferSize) {
		Array<Int8> dummy(bufferSize);
		m_storage.push_back({ name, dynamic_cast<RenderBase*>(factory->CreateBuffer(BufferType::BT_UNIFORM, 1, static_cast<Int32>(dummy.size()), dummy.data())), {}, true });
	}

	void HighRenderStorage::InitResourceAsTarget(IRenderResourceFactory* factory, const String& name, TextureType type, TextureFormat format, Int32 width, Int32 height) {
		m_storage.push_back({ name, dynamic_cast<RenderBase*>(factory->CreateTarget(type, format, width, height)), {}, true });
	}

	void HighRenderStorage::InitResourceAsTarget(TargetResource* resource, const String& name) {
		m_storage.push_back({ name, dynamic_cast<RenderBase*>(resource), {}, false });
	}

	void HighRenderBatcher::LinkWithStorage(const HighRenderStorage& storage, const String& name, EnumFlags<HRS_Tag> tags) {
		auto itRes = std::find_if(storage.m_storage.begin(), storage.m_storage.end(), [&](const HRS_Resource& res) { res.name == name; });
		if (itRes == storage.m_storage.end()) {
			throw EngineException("[HighRenderBatcher] HighRenderStorage doesn't keep a resource with the tag - " + name);
		}

		auto itNode = std::find_if(m_nodes.begin(), m_nodes.end(), [&](const HRS_ResourceNode& node) { node.resource->name == name; });
		if (itNode == m_nodes.end()) {
			m_nodes.push_back({ &(*itRes), nullptr, nullptr, tags });
			itNode = m_nodes.end() - 1;

			auto itList = std::find_if(m_topNodes.begin(), m_topNodes.end(), [&](const HRS_ResourceNode*& node) { node->resource->data->Is(*(*itRes).data); });
			if (itList == m_topNodes.end()) {
				(*itNode).last = &(*itNode);
				m_topNodes.push_back(&(*itNode));
			} else {
				HRS_ResourceNode* lastNode = (*itList)->last;
				lastNode->next = &(*itNode);
			}
		}
	}

	//template<class TResourceClass>
	//inline constexpr Array<TResourceClass*> GenerateArrayOfResources(HRS_ResourceNode* list, EnumFlags<HRS_Tag> filter, 
	//	std::function<TResourceClass*(RenderBase* data)> caster) {
	//	Array<TResourceClass*> result;
	//	for (HRS_ResourceNode* node = list; node->next != nullptr; node = node->next) {
	//		if (static_cast<bool>(node->resource->tags & filter)) {
	//			result.push_back(caster(node->resource->data));
	//		}
	//	}
	//	return result;
	//}

	//template<>
	//Array<IStateResourceData*> HighRenderBatcher::QueryResources(EnumFlags<HRS_Tag> tags) const {
	//	auto itList = std::find_if(m_topNodes.begin(), m_topNodes.end(), [&](const HRS_ResourceNode* node) {
	//		node->resource->data->Is(ResourceIdentifier::RI_STATE);
	//	});

	//	if (itList != m_topNodes.end()) {
	//		return GenerateArrayOfResources<IStateResourceData>(*itList, tags, [](RenderBase* data) {
	//			return dynamic_cast<IStateResourceData*>(data);
	//		});
	//	}
	//	return {};
	//}

	//template<>
	//Array<IBufferResourceData*> HighRenderBatcher::QueryResources(EnumFlags<HRS_Tag> tags) const {
	//	auto itList = std::find_if(m_topNodes.begin(), m_topNodes.end(), [&](const HRS_ResourceNode*& node) {
	//		node->resource->data->Is(ResourceIdentifier::RI_BUFFER);
	//	});

	//	if (itList != m_topNodes.end()) {
	//		return GenerateArrayOfResources<IBufferResourceData>(*itList, tags, [](RenderBase* data) {
	//			return dynamic_cast<IBufferResourceData*>(data);
	//		});
	//	}
	//	return {};
	//}

	//template<>
	//Array<ITargetResourceData*> HighRenderBatcher::QueryResources(EnumFlags<HRS_Tag> tags) const {
	//	auto itList = std::find_if(m_topNodes.begin(), m_topNodes.end(), [&](const HRS_ResourceNode*& node) {
	//		node->resource->data->Is(ResourceIdentifier::RI_TARGET);
	//	});

	//	if (itList != m_topNodes.end()) {
	//		return GenerateArrayOfResources<ITargetResourceData>(*itList, tags, [](RenderBase* data) {
	//			return dynamic_cast<ITargetResourceData*>(data);
	//		});
	//	}
	//	return {};
	//}

	//template<>
	//Array<ITextureResourceData*> HighRenderBatcher::QueryResources(EnumFlags<HRS_Tag> tags) const {
	//	auto itList = std::find_if(m_topNodes.begin(), m_topNodes.end(), [&](const HRS_ResourceNode*& node) {
	//		node->resource->data->Is(ResourceIdentifier::RI_TARGET);
	//	});

	//	if (itList != m_topNodes.end()) {
	//		return GenerateArrayOfResources<ITextureResourceData>(*itList, tags, [](RenderBase* data) {
	//			return dynamic_cast<ITargetResourceData*>(data)->GetTextureResource();
	//		});
	//	}
	//	return {};
	//}
}