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

		virtual ~ResourceSlot() {
			if (IsAssociatedWith(BatchSlot::BS_SLOT_DELETABLE)) {
				DELETE_OBJECT(m_resource);
			}
		}

		explicit ResourceSlot(const ResourceSlot&) noexcept = delete;
		ResourceSlot& operator=(const ResourceSlot&) noexcept = delete;

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

	class States : public IBindableResourceStandaloneStageBatch, public IBindableResourceShaderStageBatch {
	public:
		States();
		virtual ~States() = default;

		void Bind(BatchSlot batchId, IRenderStage* stage) override;
		void Bind(BatchSlot batchId, IRenderPipeline* pipeline) override;
		void InitNewResource(EnumFlags<BatchSlot> batchIds, StateType type, StateData data);

	private:
		Array<ResourceSlot<IStateResourceData>> m_batch;
	};
}

#endif // !RENDER_BUFFER_H
