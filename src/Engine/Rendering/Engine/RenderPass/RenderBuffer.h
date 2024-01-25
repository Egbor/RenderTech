#ifndef RENDER_BUFFER_H
#define RENDER_BUFFER_H

#include "Engine/Core/Core.h"

namespace Engine {
#define RBS_SLOT1 0x01
#define RBS_SLOT2 0x02
#define RBS_SLOT_ALL 0xFF

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

	class Samplers {
	public:
		Samplers();
		virtual ~Samplers() = default;

		Int32 InitNewResource(Int32 slot);
		const Array<AbstractSamplerState*> GetResources(Int32 slot) const;

	private:
		Array<ResourceWrapper<AbstractSamplerState>> m_states;
	};
}

#endif // !RENDER_BUFFER_H
