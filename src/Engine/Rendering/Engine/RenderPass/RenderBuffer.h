#ifndef RENDER_BUFFER_H
#define RENDER_BUFFER_H

#include "Engine/Core/Core.h"
#include "Engine/Core/Utils/Enum.h"

namespace Engine {
	enum class BatchSlot {
		BS_SLOT_1 = 0x01,
		BS_SLOT_2 = 0x02,
		BS_SLOT_3 = 0x04,
		BS_SLOT_ALL = 0xFF,
		BS_SLOT_DELETABLE = 0x100
	};

	//template<class TResourceData>
	//struct ResourceWrapper {
	//public:
	//	ResourceWrapper(TResourceData* resource, Int32 slot, bool isNative) noexcept
	//		: m_resource(resource), m_slots(slot), m_isNative(isNative) {
	//	
	//	}

	//	ResourceWrapper(ResourceWrapper&& wrapper) noexcept
	//		: m_resource(std::move(wrapper.m_resource))
	//		, m_isNative(std::move(wrapper.m_isNative)) {
	//		wrapper.m_isNative = false;
	//	}

	//	ResourceWrapper& operator=(ResourceWrapper&& wrapper) noexcept {
	//		m_resource = std::move(wrapper.m_resource);
	//		m_isNative = std::move(wrapper.m_isNative);
	//		return *this;
	//	}

	//	virtual ~ResourceWrapper() noexcept {
	//		if (m_isNative) {
	//			DELETE_OBJECT(m_resource);
	//		}
	//	}

	//	TResourceData* GetResource() const {
	//		return m_resource;
	//	}

	//	bool IsAssociatedWith(Int32 slot) const {
	//		return !!(m_slots & slot);
	//	}

	//private:
	//	TResourceData* m_resource;
	//	Int32 m_slots;
	//	bool m_isNative;
	//};

	template<class TResourceData>
	struct ResourceSlot {
		ResourceSlot(EnumFlags<BatchSlot> batchIds, TResourceData* resource) 
			: m_batchIds(batchIds), m_resource(resource) {

		}

		virtual ~ResourceSlot() {
			if (IsAssociatedWith(BatchSlot::BS_SLOT_DELETABLE)) {
				DELETE_OBJECT(m_resource);
			}
		}

		TResourceData* GetResource() const {
			return m_resource;
		}

		bool IsAssociatedWith(BatchSlot slot) const {
			return !!(m_slots & slot);
		}

	private:
		TResourceData* m_resource;
		EnumFlags<BatchSlot> m_batchIds;
	};

	class IBindableResourceShaderStageBatch {
	public:
		virtual ~IBindableResourceShaderStageBatch() = default;
		virtual void Bind(BatchSlot batchId, IRenderStage* stage) = 0;
	};

	class IBindableResourceStandaloneStageBatch {
	public:
		virtual ~IBindableResourceStandaloneStageBatch() = default;
		virtual void Bind(BatchSlot batchId, IRenderPipeline* pipeline) = 0;
	};

	class GBuffer : public IBindableResourceStandaloneStageBatch, public IBindableResourceShaderStageBatch {
	public:
		GBuffer();
		virtual ~GBuffer() = default;

		void Bind(BatchSlot batchId, IRenderStage* stage) override;
		void Bind(BatchSlot batchId, IRenderPipeline* pipeline) override;

		void InitNewResource(EnumFlags<BatchSlot> batchIds, ITargetResourceData* resource);
		void InitNewResource(EnumFlags<BatchSlot> batchIdx, TextureType type, TextureFormat format, Int32 width, Int32 height);

		void Clear();

	private:
		Array<ResourceSlot<ITargetResourceData>> m_batch;
	};

	class UBuffer : public IBindableResourceShaderStageBatch {
	public:
		UBuffer();
		virtual ~UBuffer() = default;

		void Bind(BatchSlot batchId, IRenderStage* stage) override;
		void InitNewResource(EnumFlags<BatchSlot> batchIds, Int32 bufferSize, Int32* outId);
		void Update(Int32 id);

		RawData GetBufferData(Int32 id) const;

	private:
		Array<ResourceSlot<IBufferResourceData>> m_batch;
	};

	class Samplers : public IBindableResourceShaderStageBatch {
	public:
		Samplers();
		virtual ~Samplers() = default;

		void Bind(BatchSlot batchId, IRenderStage* stage) override;
		void InitNewResource(EnumFlags<BatchSlot> batchIds);

	private:
		Array<ResourceSlot<IStateResourceData>> m_batch;
	};
}

#endif // !RENDER_BUFFER_H
