#ifndef RENDER_BUFFER_H
#define RENDER_BUFFER_H

#include "Engine/Core/Core.h"
#include "Engine/Core/Utils/Enum.h"
#include "Engine/Core/Utils/Event.h"

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
		String name;
		RenderBase* data;
		Int8 _aligment[8];
		bool isDeletable;
	};

	struct HRS_ResourceNode {
		const HRS_Resource* resource;
		HRS_ResourceNode* next;
		HRS_ResourceNode* last;
		EnumFlags<HRS_Tag> tags;
	};

	class HighRenderStorage {
		friend class HighRenderBatcher;

	public:
		HighRenderStorage() = default;
		~HighRenderStorage();

		void InitResourceAsState(IRenderResourceFactory* factory, const String& name, StateType type, StateData data);
		void InitResourceAsBuffer(IRenderResourceFactory* factory, const String& name, Int32 bufferSize);
		void InitResourceAsTarget(IRenderResourceFactory* factory, const String& name, TextureType type, TextureFormat format, Int32 width, Int32 height);
		void InitResourceAsTarget(TargetResource* resource, const String& name);

	private:
		Array<HRS_Resource> m_storage;
	};

	template<class TResourceClass>
	struct NamePlusResourceWrapper {
		const String& name;
		TResourceClass* resource;
	};

	class HighRenderBatcher {
	public:
		HighRenderBatcher() = default;
		~HighRenderBatcher() = default;

		void LinkWithStorage(const HighRenderStorage& storage, const String& name, EnumFlags<HRS_Tag> tags);

		template<class TResourceClass>
		Array<TResourceClass*> QueryResources(EnumFlags<HRS_Tag> tags) const {
			ResourceIdentifier id = TResourceClass::GetResourceIdentifier();

			if (id == ResourceIdentifier::RI_TEXTURE) {
				return SelectResources<TextureResource*>(ResourceIdentifier::RI_TARGET, [&](const HRS_Resource* resource, EnumFlags<HRS_Tag> resourceTags) {
					if ((resourceTags & tags) == tags) {
						return dynamic_cast<TargetResource*>(resource->data)->GetTextureResource();
					}
				});
			}

			return SelectResources<TResourceClass*>(TResourceClass::GetResourceIdentifier(), [&](const HRS_Resource* resource, EnumFlags<HRS_Tag> resourceTags) {
				if ((resourceTags & tags) == tags) {
					return dynamic_cast<TResourceClass*>(resource->data);
				}
			});
		}

		template<class TResourceClass>
		Array<NamePlusResourceWrapper<TResourceClass>> QueryNamePlusResources() const {
			return SelectResources<TResourceClass*>(TResourceClass::GetResourceIdentifier(), [](const HRS_Resource* resource, EnumFlags<HRS_Tag> resourceTags) {
				return { resource->name, dynamic_cast<TResourceClass*>(resource->data) };
			});
		}

		template<class TResourceClass>
		TResourceClass* QueryResourceByName(const String& name) const {
			Array<TResourceClass*> resources = SelectResources<TResourceClass*>(TResourceClass::GetResourceIdentifier(), [&](const HRS_Resource* resource, EnumFlags<HRS_Tag> resourceTags) {
				if (resource->name == name) {
					return dynamic_cast<TResourceClass*>(resource->data);
				}
			});
			return resources.size() > 0 ? resources[i] : nullptr;
		}

	private:
		template<class TCastomWrapper>
		Array<TCastomWrapper> SelectResources(ResourceIdentifier id, std::function<TCastomWrapper(const HRS_Resource*, EnumFlags<HRS_Tag>)> selector) const {
			auto it = std::find_if(m_topNodes.begin(), m_topNodes.end(), [&](const HRS_ResourceNode* node) {
				node->resource->data->Is(id);
			});

			Array<TCastomWrapper> outcome;

			if (it != m_topNodes.end()) {
				for (HRS_ResourceNode* node = *it; node->next != nullptr; node = node->next) {
					outcome.push_back(selector(node->resource, node->tags));
				}
			}
			return outcome;
		}

		Array<HRS_ResourceNode*> m_topNodes;
		Array<HRS_ResourceNode> m_nodes;
	};
}

#endif // !RENDER_BUFFER_H
