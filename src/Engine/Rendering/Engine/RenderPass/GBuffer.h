#ifndef GBUFFER_H
#define GBUFFER_H

#include "Engine/Core/Core.h"

namespace Engine {
	template<class TResourceData>
	struct ResourceWrapper {
	public:
		ResourceWrapper(TResourceData* resource, bool isNative) noexcept
			: m_resource(resource), m_isNative(isNative) {
		
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

	private:
		TResourceData* m_resource;
		bool m_isNative;
	};

	struct GBuffer {
	public:
		GBuffer();
		virtual ~GBuffer() = default;

		Int32 InitNewResource(ITargetResourceData* resource);
		Int32 InitNewResource(TextureType type, TextureFormat format, Int32 width, Int32 height);
		void Clear();

		const Array<ITargetResourceData*> GetTargets() const;
		const Array<ITextureResourceData*> GetTextures() const;

	private:
		Array<ResourceWrapper<ITargetResourceData>> m_targets;
	};

	struct UBuffer {
	public:
		UBuffer();
		virtual ~UBuffer() = default;

		Int32 InitNewResource(Int32 bufferSize);
		void Update(Int32 index);

		RawData GetBufferData(Int32 index) const;
		const Array<IBufferResourceData*> GetBuffers() const;

	private:
		Array<ResourceWrapper<IBufferResourceData>> m_buffers;
	};

	struct Samplers {
	public:
		Samplers();
		virtual ~Samplers() = default;

		Int32 InitNewResource();
		const Array<AbstractSamplerState*> GetResources() const;

	private:
		Array<ResourceWrapper<AbstractSamplerState>> m_states;
	};
}

#endif // !GBUFFER_H
