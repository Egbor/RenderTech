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
		m_storage.push_back({ name, resource, {}, true });
		return resource;
	}

	BufferResource* HighRenderStorage::InitResourceAsBuffer(IRenderResourceFactory* factory, const String& name, Int32 bufferSize) {
		Array<Int8> dummy(bufferSize);
		BufferResource* resource = factory->CreateBuffer(BufferType::BT_UNIFORM, name, 1, static_cast<Int32>(dummy.size()), dummy.data());
		m_storage.push_back({ name, resource, {}, true });
		return resource;
	}

	TargetResource* HighRenderStorage::InitResourceAsTarget(IRenderResourceFactory* factory, const String& name, TextureType type, TextureFormat format, Int32 width, Int32 height) {
		TargetResource* resource = factory->CreateTarget(type, name, format, width, height);
		m_storage.push_back({ name, resource, {}, true });
		return resource;
	}

	TargetResource* HighRenderStorage::InitResourceAsTarget(TargetResource* resource, const String& name) {
		m_storage.push_back({ name, resource, {}, false });
		return resource;
	}

	void HighRenderBatcher::LinkWithStorage(const HighRenderStorage& storage, const String& name, EnumFlags<HRS_Tag> tags) {
		auto itRes = std::find_if(storage.m_storage.begin(), storage.m_storage.end(), [&](const HRS_Resource& res) { return res.data->GetName() == name; });
		if (itRes == storage.m_storage.end()) {
			throw EngineException("[HighRenderBatcher] HighRenderStorage doesn't keep a resource with the tag - " + name);
		}

		auto itNode = std::find_if(m_nodes.begin(), m_nodes.end(), [&](const HRS_ResourceNode& node) { return node.resource->data->GetName() == name; });
		if (itNode == m_nodes.end()) {
			m_nodes.push_back({ &(*itRes), nullptr, nullptr, tags });
			itNode = m_nodes.end() - 1;

			auto itList = std::find_if(m_topNodes.begin(), m_topNodes.end(), [&](const HRS_ResourceNode* node) { return node->resource->data->Is(*(*itRes).data); });
			if (itList == m_topNodes.end()) {
				(*itNode).last = &(*itNode);
				m_topNodes.push_back(&(*itNode));
			} else {
				HRS_ResourceNode* lastNode = (*itList)->last;
				lastNode->next = &(*itNode);
			}
		}
	}
}