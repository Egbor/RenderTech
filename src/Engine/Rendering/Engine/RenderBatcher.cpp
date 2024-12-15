#include "Engine/Rendering/Engine/RenderBatcher.h"
#include "Engine/Core/System/Exception/EngineException.h"

namespace Engine {
	HighRenderStorage::~HighRenderStorage() {
		for (Size i = 0; i < m_storage.size(); i++) {
			if (m_storage[i].isDeletable) {
				DELETE_OBJECT(m_storage[i].data);
			}
		}
	}

	StateResource* HighRenderStorage::InitResourceAsState(IRenderResourceFactory* factory, const String& name, StateType type, StateData data) {
		StateResource* resource = factory->CreateState(type, name, data);
		m_storage.emplace_back(resource, true);
		return resource;
	}

	BufferResource* HighRenderStorage::InitResourceAsBuffer(IRenderResourceFactory* factory, const String& name, Int32 bufferSize) {
		Array<Int8> dummy(bufferSize);
		BufferResource* resource = factory->CreateBuffer(BufferType::BT_UNIFORM, name, 1, static_cast<Int32>(dummy.size()), dummy.data());
		m_storage.emplace_back(resource, true);
		return resource;
	}

	TargetResource* HighRenderStorage::InitResourceAsTarget(IRenderResourceFactory* factory, const String& name, TextureType type, TextureFormat format, Int32 width, Int32 height) {
		TargetResource* resource = factory->CreateTarget(type, name, format, width, height);
		m_storage.emplace_back(resource, true);
		return resource;
	}

	TargetResource* HighRenderStorage::InitResourceAsTarget(TargetResource* resource, const String& name) {
		m_storage.emplace_back(resource, false);
		return resource;
	}

	bool HighRenderStorage::IsEmpty() const {
		return m_storage.empty();
	}

	HighRenderBatcher::HighRenderBatcher() {
		m_nodes.reserve(32);
		m_tops.reserve(4);
	}

	void HighRenderBatcher::AddNewLinkToStorage(const HighRenderStorage& storage, const String& name, EnumFlags<HRS_Tag> tags) {
		auto itRes = std::find_if(storage.m_storage.begin(), storage.m_storage.end(), [&](const HRS_Resource& res) { 
			return res.data->GetName() == name; 
		});

		if (itRes == storage.m_storage.end()) {
			throw EngineException("[HighRenderBatcher] HighRenderStorage doesn't keep a resource with the tag - " + name);
		}

		const HRS_Resource* resource = &(*itRes);
		m_nodes.emplace_back(resource, tags);

		auto itNode = m_nodes.end() - 1;
		auto itList = std::find_if(m_tops.begin(), m_tops.end(), [&](const HRS_ResourceNode* node) { 
			return node->resource->data->Is(*(resource->data)); 
		});

		if (itList == m_tops.end()) {
			(*itNode).last = &(*itNode);
			m_tops.push_back(&(*itNode));
		} else {
			HRS_ResourceNode* lastNode = (*itList)->last;
			lastNode->next = &(*itNode);
		}
	}

	Array<RenderBase*> HighRenderBatcher::SelectResources(ResourceIdentifier id, std::function<RenderBase*(HRS_ResourceNode*)> selector) const {
		auto it = std::find_if(m_tops.begin(), m_tops.end(), [&](const HRS_ResourceNode* node) { return node->resource->data->Is(id); });

		Array<RenderBase*> result;
		if (it != m_tops.end()) {
			for (HRS_ResourceNode* node = *it; node->next != nullptr; node = node->next) {
				RenderBase* resource = selector(node);
				if (resource != nullptr) {
					result.push_back(resource);
				}
			}
		}
		return result;
	}

	RenderBase* HighRenderBatcher::DefaultSelector(const HRS_ResourceNode* node, EnumFlags<HRS_Tag> tag) {
		if ((node->tags & tag) == tag) {
			return node->resource->data;
		}
		return nullptr;
	}

	RenderBase* HighRenderBatcher::TextureSelector(const HRS_ResourceNode* node, EnumFlags<HRS_Tag> tag) {
		if ((node->tags & tag) == tag) {
			return dynamic_cast<TargetResource*>(node->resource->data)->GetTextureResource();
		}
		return nullptr;
	}

	//void HighRenderBatcher::LinkWithStorage(const HighRenderStorage& storage, const String& name, EnumFlags<HRS_Tag> tags) {
	//	auto itRes = std::find_if(storage.m_storage.begin(), storage.m_storage.end(), [&](const HRS_Resource& res) { return res.data->GetName() == name; });
	//	if (itRes == storage.m_storage.end()) {
	//		throw EngineException("[HighRenderBatcher] HighRenderStorage doesn't keep a resource with the tag - " + name);
	//	}

	//	auto itNode = std::find_if(m_nodes.begin(), m_nodes.end(), [&](const HRS_ResourceNode& node) { return node.resource->data->GetName() == name; });
	//	if (itNode == m_nodes.end()) {
	//		m_nodes.push_back({ &(*itRes), nullptr, nullptr, tags });
	//		itNode = m_nodes.end() - 1;

	//		auto itList = std::find_if(m_topNodes.begin(), m_topNodes.end(), [&](const HRS_ResourceNode* node) { return node->resource->data->Is(*(*itRes).data); });
	//		if (itList == m_topNodes.end()) {
	//			(*itNode).last = &(*itNode);
	//			m_topNodes.push_back(&(*itNode));
	//		} else {
	//			HRS_ResourceNode* lastNode = (*itList)->last;
	//			lastNode->next = &(*itNode);
	//		}
	//	}
	//}
}