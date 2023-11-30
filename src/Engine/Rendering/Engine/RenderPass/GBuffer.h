#ifndef GBUFFER_H
#define GBUFFER_H

#include "Engine/Core/Core.h"

namespace Engine {
	template<class TResourceData>
	struct BufferResourceWrapper {
	public:
		BufferResourceWrapper(TResourceData* resource, bool isNative) noexcept
			: m_resource(resource), m_isNative(isNative) {
		
		}

		BufferResourceWrapper(BufferResourceWrapper&& wrapper) noexcept
			: m_resource(std::move(wrapper.m_resource))
			, m_isNative(std::move(wrapper.m_isNative)) {
			wrapper.m_isNative = false;
		}

		BufferResourceWrapper& operator=(BufferResourceWrapper&& wrapper) noexcept {
			m_resource = std::move(wrapper.m_resource);
			m_isNative = std::move(wrapper.m_isNative);
			return *this;
		}

		virtual ~BufferResourceWrapper() noexcept {
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
		virtual ~GBuffer();

		Int32 Attach(ITargetResourceData* resource);
		Int32 Attach(TextureType type, TextureFormat format, Int32 width, Int32 height);
		void Clear();

		const Array<ITargetResourceData*> GetTargets() const;
		const Array<ITextureResourceData*> GetTextures() const;

	private:
		Array<BufferResourceWrapper<ITargetResourceData>> m_targets;
	};

	struct UBuffer {
	public:
		UBuffer();
		virtual ~UBuffer();

		Int32 Attach(Int32 bufferSize);
		void Update(Int32 index);

		RawData GetBufferData(Int32 index) const;
		const Array<IBufferResourceData*> GetBuffers() const;

	private:
		Array<BufferResourceWrapper<IBufferResourceData>> m_buffers;
	};
}

#endif // !GBUFFER_H
