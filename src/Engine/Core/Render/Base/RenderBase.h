#ifndef RENDER_BASE_H
#define RENDER_BASE_H

#include "Engine/Core/Render/Base/RenderDef.h"

namespace Engine {
	class IContext;

	class RenderBase {
	public:
		RenderBase(ResourceIdentifier identifier, IContext* context);
		virtual ~RenderBase() = default;

		bool Is(ResourceIdentifier identifier) const;
		bool Is(const RenderBase& object) const;

	protected:
		IContext* GetContext() const;

	private:
		IContext* m_context;
		ResourceIdentifier m_identifier;
	};
}

#endif // !RENDER_BASE_H
