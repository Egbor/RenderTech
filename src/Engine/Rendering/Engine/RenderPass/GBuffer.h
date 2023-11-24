#ifndef GBUFFER_H
#define GBUFFER_H

#include "Engine/Core/Render/Api/Interface/ITargetResource.h"
#include "Engine/Core/Render/Api/Interface/IBufferResource.h"

namespace Engine {
	class IRenderPipeline;

	struct GBuffer {
	public:
		GBuffer();
		virtual ~GBuffer();

		Int32 Attach(ITargetResourceData* resource);
		Int32 Attach(TextureType type, TextureFormat format, Int32 width, Int32 height);
		void Clear(IRenderPipeline* pipeline);

		const Array<ITargetResourceData*>& GetTargets() const;
		const Array<ITextureResourceData*> GetTextures() const;

	private:
		Array<ITargetResourceData*> m_targets;
	};

	struct UBuffer {
	public:
		UBuffer();
		virtual ~UBuffer();

		template<class BufferDesc>
		Int32 Attach(IRenderResourceFactory* factory) {
			BufferDesc bufferdesc;
			IBufferResourceData* resource = factory->CreateBuffer(BufferType::BT_UNIFORM, 1, sizeof(BufferDesc), &bufferdesc);

			m_buffers.push_back(resource);
			return static_cast<Int32>(m_buffers.size() - 1);
		}

		const Array<IBufferResourceData*>& GetBuffers() const;
		IBufferResourceData* GetBuffer(Int32 index) const;

	private:
		Array<IBufferResourceData*> m_buffers;
	};
}

#endif // !GBUFFER_H
