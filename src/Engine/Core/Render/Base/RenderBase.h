#ifndef RENDER_BASE_H
#define RENDER_BASE_H

#include "Engine/Core/Render/Base/RenderDef.h"

namespace Engine {
	class IContext;

	class RenderBase {
	public:
		RenderBase(const String& name, IContext* context, ResourceIdentifier identifier);
		virtual ~RenderBase() = default;

		bool Is(ResourceIdentifier identifier) const;
		bool Is(const RenderBase& object) const;

		const String& GetName() const;

	protected:
		IContext* GetContext() const;

	private:
		String m_name;
		IContext* m_context;
		ResourceIdentifier m_identifier;
	};
}

#endif // !RENDER_BASE_H
