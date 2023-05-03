#ifndef TARGETMAP
#define TARGETMAP

#include "Engine/Core/Render/Api/Context.h"
#include "Engine/Object/Class/Target.h"

namespace Engine {
	class TargetMap {
	private:
		Context* m_context;
		Array<Target*> m_targets;

	public:
		TargetMap(Context* context);
		virtual ~TargetMap();

		void AddTarget(RenderOutput slot, Texture2D* texture);
		void AddTarget(RenderOutput slot, TextureFormat format, UInt32 width, UInt32 height);
		void AddTarget(RenderOutput slot, Target* target);

		void RemoveTarget(RenderOutput slot);
		Target* GetTarget(RenderOutput slot) const;
	};
}

#endif // !TARGETMAP
