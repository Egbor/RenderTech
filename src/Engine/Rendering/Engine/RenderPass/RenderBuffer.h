#ifndef RENDER_BUFFER_H
#define RENDER_BUFFER_H

#include "Engine/Core/Core.h"
#include "Engine/Core/Utils/Enum.h"

namespace Engine {
	enum class BatchSlot : UInt32 {
		BS_SLOT_1 = 0x01,
		BS_SLOT_2 = 0x02,
		BS_SLOT_3 = 0x04,
		BS_SLOT_ALL = 0xFF
	};

	template<class TResourceData>
	struct ResourceWrapper {
	public:
		ResourceWrapper(TResourceData* resource, Int32 slot, bool isNative) noexcept
			: m_resource(resource), m_slots(slot), m_isNative(isNative) {
		
		}

		ResourceWrapper(ResourceWrapper&& wrapper) noexcept
			: m_resource(std::move(wrapper.m_resource))
			, m_isNative(std::move(wrapper.m_isNative)) {
			wrapper.m_isNative = false;
		}

		ResourceWrapper& operator=(ResourceWrapper&& wrapper) noexcept {
			m_resource = std::move(wrapper.m_resource);
			m_isNative = std::move(wrapper.m_isNative);
			return *this;
		}

		virtual ~ResourceWrapper() noexcept {
			if (m_isNative) {
				DELETE_OBJECT(m_resource);
			}
		}

		TResourceData* GetResource() const {
			return m_resource;
		}

		bool IsAssociatedWith(Int32 slot) const {
			return !!(m_slots & slot);
		}

	private:
		TResourceData* m_resource;
		Int32 m_slots;
		bool m_isNative;
	};

	template<class TResourceData>
	struct ResourceSlot {
		ResourceSlot(Int32 batchIds, TResourceData* resource) 
			: m_batchIds(batchIds), m_resource(resource) {

		}

		virtual ~ResourceSlot() {
			DELETE_OBJECT(m_resource);
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
	private:
		virtual ~IBindableResourceStandaloneStageBatch() = default;
		virtual void Bind(BatchSlot batchId, IRenderPipeline* pipeline) = 0;
	};

	class GBuffer {
	public:
		GBuffer();
		virtual ~GBuffer() = default;

		Int32 InitNewResource(Int32 slot, ITargetResourceData* resource);
		Int32 InitNewResource(Int32 slot, TextureType type, TextureFormat format, Int32 width, Int32 height);
		void Clear();

		const Array<ITargetResourceData*> GetTargets() const;
		const Array<ITextureResourceData*> GetResources(Int32 slot) const;

	private:
		Array<ResourceWrapper<ITargetResourceData>> m_targets;
	};

	class UBuffer {
	public:
		UBuffer();
		virtual ~UBuffer() = default;

		Int32 InitNewResource(Int32 slot, Int32 bufferSize);
		void Update(Int32 id);

		RawData GetBufferData(Int32 id) const;
		const Array<IBufferResourceData*> GetResources(Int32 slot) const;

	private:
		Array<ResourceWrapper<IBufferResourceData>> m_buffers;
	};

	class Samplers : public IBindableResourceShaderStageBatch {
	public:
		Samplers();
		virtual ~Samplers() = default;

		void Bind(BatchSlot batchId, IRenderStage* stage) override;
		void InitNewResource(EnumFlags<BatchSlot> batchIds);

	private:
		Array<ResourceSlot<ISamplerStateData>> m_batch;
	};
}

#endif // !RENDER_BUFFER_H
