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

		Int32 Attach(IBufferResourceData* resource);

		const Array<IBufferResourceData*>& GetBuffers() const;
		IBufferResourceData* GetBuffer(Int32 index) const;

	private:
		Array<IBufferResourceData*> m_buffers;
	};
}

#endif // !GBUFFER_H
