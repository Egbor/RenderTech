#ifndef GBUFFER_H
#define GBUFFER_H

#include "Engine/Core/Core.h"

namespace Engine {
	struct GBuffer {
	public:
		GBuffer();
		virtual ~GBuffer();

		Int32 Attach(ITargetResourceData* resource);
		Int32 Attach(TextureType type, TextureFormat format, Int32 width, Int32 height);
		void Clear();

		const Array<ITargetResourceData*>& GetTargets() const;
		const Array<ITextureResourceData*> GetTextures() const;

	private:
		Array<ITargetResourceData*> m_targets;
	};

	struct UBuffer {
	public:
		UBuffer();
		virtual ~UBuffer();

		Int32 Attach(Int32 bufferSize);
		void Update(Int32 index);

		RawData GetBufferData(Int32 index) const;
		const Array<IBufferResourceData*>& GetBuffers() const;

	private:
		Array<IBufferResourceData*> m_buffers;
	};
}

#endif // !GBUFFER_H
