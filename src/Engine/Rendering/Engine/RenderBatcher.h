#ifndef RENDER_BUFFER_H
#define RENDER_BUFFER_H

#include "Engine/Core/Core.h"
#include "Engine/Core/Utils/Enum.h"
#include "Engine/Core/Utils/Event.h"

#include <algorithm>

namespace Engine {
	enum class HRS_Tag {
		HRS_NONE = 0x00000000,
		HRS_STATE = 0x00000001,
		HRS_TARGET = 0x00000002,
		HRS_VS_STAGE = 0x00000004,
		HRS_PS_STAGE = 0x00000008,
		HRS_STANDALONE = 0x00000010,
		HRS_ADDITION_1 = 0x40000000,
		HRS_ADDITION_2 = 0x80000000
	};

	constexpr EnumFlags<HRS_Tag> TAG_GBUFFER = HRS_Tag::HRS_TARGET | HRS_Tag::HRS_PS_STAGE;
	constexpr EnumFlags<HRS_Tag> TAG_SAMPLER = HRS_Tag::HRS_STATE | HRS_Tag::HRS_PS_STAGE;
	constexpr EnumFlags<HRS_Tag> TAG_STATE = HRS_Tag::HRS_STATE | HRS_Tag::HRS_STANDALONE;
	constexpr EnumFlags<HRS_Tag> TAG_VS_PLUS_PS = HRS_Tag::HRS_VS_STAGE | HRS_Tag::HRS_PS_STAGE;
	constexpr EnumFlags<HRS_Tag> TAG_ANY = ~HRS_Tag::HRS_NONE;

	struct HRS_Resource {
		RenderBase* data;
		bool isDeletable;

		HRS_Resource(RenderBase* data, bool isDeletable) 
			: data(data), isDeletable(isDeletable) {}
	};

	struct HRS_ResourceNode {
		const HRS_Resource* resource;
		HRS_ResourceNode* next;
		HRS_ResourceNode* last;
		EnumFlags<HRS_Tag> tags;

		HRS_ResourceNode(const HRS_Resource* resource, EnumFlags<HRS_Tag> tags)
			: resource(resource), tags(tags), next(nullptr), last(nullptr) {}
	};

	class HighRenderStorage {
		friend class HighRenderBatcher;

	public:
		HighRenderStorage() = default;
		~HighRenderStorage();

		StateResource* InitResourceAsState(IRenderResourceFactory* factory, const String& name, StateType type, StateData data);
		BufferResource* InitResourceAsBuffer(IRenderResourceFactory* factory, const String& name, Int32 bufferSize);
		TargetResource* InitResourceAsTarget(IRenderResourceFactory* factory, const String& name, TextureType type, TextureFormat format, Int32 width, Int32 height);
		TargetResource* InitResourceAsTarget(TargetResource* resource, const String& name);

		bool IsEmpty() const;

	private:
		Array<HRS_Resource> m_storage;
	};

	class HighRenderBatcher {
	public:
		HighRenderBatcher();

		void AddNewLinkToStorage(const HighRenderStorage& storage, const String& name, EnumFlags<HRS_Tag> tags);

		template<class TResourceClass>
		Array<TResourceClass*> QueryResources(EnumFlags<HRS_Tag> tags) const {
			Array<RenderBase*> resources = QueryResources(TResourceClass::GetResourceIdentifier(), [tags](const HRS_ResourceNode* node) { return !!(node->tags & tags); });
			Array<TResourceClass*> result(resources.size());

			std::transform(resources.crbegin(), resources.crend(), result.begin(), [](RenderBase* item) { return dynamic_cast<TResourceClass*>(item); });
			return result;
		}

		template<class TResourceClass>
		TResourceClass* QueryResourceByName(const String& name) const {
			Array<RenderBase*> resources = QueryResources(TResourceClass::GetResourceIdentifier(), [&name](const HRS_ResourceNode* node) { return node->resource->data->GetName() == name; });
			return resources.size() > 0 ? dynamic_cast<TResourceClass*>(resources[resources.size() - 1]) : nullptr;
		}

	private:
		Array<RenderBase*> SelectResources(ResourceIdentifier id, std::function<RenderBase* (HRS_ResourceNode*)> selector) const;
		Array<RenderBase*> QueryResources(ResourceIdentifier id, std::function<bool(const HRS_ResourceNode*)> filter) const;

		static RenderBase* DefaultSelector(const HRS_ResourceNode* node, std::function<bool(const HRS_ResourceNode*)> filter);
		static RenderBase* TextureSelector(const HRS_ResourceNode* node, std::function<bool(const HRS_ResourceNode*)> filter);

		Array<HRS_ResourceNode*> m_tops;
		Array<HRS_ResourceNode> m_nodes;
	};
}

#endif // !RENDER_BUFFER_H
