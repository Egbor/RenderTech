#ifndef RENDER_BUFFER_H
#define RENDER_BUFFER_H

#include "Engine/Core/Core.h"
#include "Engine/Core/Utils/Enum.h"

namespace Engine {
	enum class BatchSlot {
		BS_SLOT_1 = 0x01,
		BS_SLOT_2 = 0x02,
		BS_SLOT_3 = 0x04,
		BS_SLOT_4 = 0x08,
		BS_SLOT_5 = 0x10,
		BS_SLOT_6 = 0x20,
		BS_SLOT_7 = 0x40,
		BS_SLOT_8 = 0x80,
		BS_SLOT_ALL = 0xFF,
		BS_SLOT_NULL = 0x100,
		BS_DELETABLE = 0x1000
	};

	template<class TResourceData>
	class ResourceSlot {
	public:
		ResourceSlot(EnumFlags<BatchSlot> batchIds, TResourceData* resource) noexcept
			: m_batchIds(batchIds), m_resource(resource) {

		}

		ResourceSlot(ResourceSlot&& other) noexcept
			: m_batchIds(std::move(other.m_batchIds)), m_resource(nullptr) {
			std::swap(m_resource, other.m_resource);
		}

		ResourceSlot& operator=(ResourceSlot&& other) noexcept {
			std::swap(m_batchIds, other.m_batchIds);
			std::swap(m_resource, other.m_resource);

			return *this;
		}

		~ResourceSlot() {
			if (IsAssociatedWith(BatchSlot::BS_DELETABLE)) {
				DELETE_OBJECT(m_resource);
			}
		}

		explicit ResourceSlot(const ResourceSlot&) noexcept = delete;
		ResourceSlot& operator=(const ResourceSlot&) noexcept = delete;

		TResourceData* operator->() const {
			return m_resource;
		}

		TResourceData* GetResource() const {
			return m_resource;
		}

		bool IsAssociatedWith(BatchSlot slot) const {
			return static_cast<bool>(m_batchIds & slot);
		}

	private:
		TResourceData* m_resource;
		EnumFlags<BatchSlot> m_batchIds;
	};

	template<class TResourceData>
	class Batch {
	public:
		Batch() = default;
		~Batch() = default;

		constexpr Int32 Insert(EnumFlags<BatchSlot> slots, TResourceData* resource) {
			m_resources.push_back(ResourceSlot(slots, resource));
			return static_cast<Int32>(m_resources.size() - 1);
		}

		constexpr TResourceData* Get(Int32 resourceId) const {
			return m_resources[resourceId].GetResource();
		}

		constexpr void HandleResources(BatchSlot slot, std::function<void(TResourceData*)> callback) const {
			for (Size i = 0; i < m_resources.size(); i++) {
				if (m_resources[i].IsAssociatedWith(slot)) {
					callback(m_resources[i].GetResource());
				}
			}
		}

	private:
		Array<ResourceSlot<TResourceData>> m_resources;
	};

	class RenderResourcesStorage {
	public:
		RenderResourcesStorage() = default;
		~RenderResourcesStorage() = default;

		template<class TResourceData>
		TResourceData* GetResourceFromBatchByTag(const String& tag) const;

		template<class TResourceData>
		void HandleResources(BatchSlot slot, std::function<void(TResourceData*)> callback) const;

		void InitResourceForBatchOfStates(IRenderResourceFactory* factory, EnumFlags<BatchSlot> slots, const String& tag, StateType type, StateData data);
		void InitResourceForBatchOfBuffers(IRenderResourceFactory* factory, EnumFlags<BatchSlot> slots, const String& tag, Int32 bufferSize);
		void InitResourceForBatchOfTargets(IRenderResourceFactory* factory, EnumFlags<BatchSlot> slots, const String& tag, TextureType type, TextureFormat format, Int32 width, Int32 height);
		void InitResourceForBatchOfTargets(ITargetResourceData* resource, EnumFlags<BatchSlot> slots, const String& tag);

	private:
		Batch<IStateResourceData> m_batchOfStates;
		Batch<ITargetResourceData> m_batchOfTargets;
		Batch<IBufferResourceData> m_batchOfBuffers;
		Batch<ITextureResourceData> m_batchOfTextures;

		Map<String, Int32> m_resourcesIds;
	};

	//class IBindableResourceShaderStageBatch {
	//public:
	//	virtual ~IBindableResourceShaderStageBatch() = default;
	//	virtual void Bind(BatchSlot batchId, IRenderStage* stage) = 0;
	//};

	//class IBindableResourceStandaloneStageBatch {
	//public:
	//	virtual ~IBindableResourceStandaloneStageBatch() = default;
	//	virtual void Bind(BatchSlot batchId, IRenderPipeline* pipeline) = 0;
	//};

	//class GBuffer : public IBindableResourceStandaloneStageBatch, public IBindableResourceShaderStageBatch {
	//public:
	//	GBuffer();
	//	virtual ~GBuffer() = default;

	//	void Bind(BatchSlot batchId, IRenderStage* stage) override;
	//	void Bind(BatchSlot batchId, IRenderPipeline* pipeline) override;

	//	void InitNewResource(EnumFlags<BatchSlot> batchIds, Int32& outId, ITargetResourceData* resource);
	//	void InitNewResource(EnumFlags<BatchSlot> batchIdx, Int32& outId, TextureType type, TextureFormat format, Int32 width, Int32 height);

	//	void Clear(BatchSlot batchId, bool enableDepthClear, bool enableStencilClear, UInt32 stencilClearValue);
	//	ITextureResourceData* GetTargetData(Int32 id) const;
	//	ITargetResourceData* GetTarget(Int32 id) const;

	//private:
	//	Array<ResourceSlot<ITargetResourceData>> m_batch;
	//};

	//class UBuffer : public IBindableResourceShaderStageBatch {
	//public:
	//	UBuffer();
	//	virtual ~UBuffer() = default;

	//	void Bind(BatchSlot batchId, IRenderStage* stage) override;
	//	void InitNewResource(EnumFlags<BatchSlot> batchIds, Int32 bufferSize, Int32& outId);
	//	void Update(Int32 id);

	//	RawData GetBufferData(Int32 id) const;

	//private:
	//	Array<ResourceSlot<IBufferResourceData>> m_batch;
	//};

	//class States : public IBindableResourceStandaloneStageBatch, public IBindableResourceShaderStageBatch {
	//public:
	//	States();
	//	virtual ~States() = default;

	//	void Bind(BatchSlot batchId, IRenderStage* stage) override;
	//	void Bind(BatchSlot batchId, IRenderPipeline* pipeline) override;
	//	void InitNewResource(EnumFlags<BatchSlot> batchIds, StateType type, StateData data);

	//private:
	//	Array<ResourceSlot<IStateResourceData>> m_batch;
	//};
}

#endif // !RENDER_BUFFER_H
