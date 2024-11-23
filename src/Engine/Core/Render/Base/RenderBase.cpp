#include "Engine/Core/Render/Base/RenderBase.h"

namespace Engine {
	RenderBase::RenderBase(const String& name, IContext* context, ResourceIdentifier identifier)
		: m_identifier(identifier), m_context(context) {

	}

	bool RenderBase::Is(ResourceIdentifier identifier) const {
		return m_identifier == identifier;
	}

	bool RenderBase::Is(const RenderBase& object) const {
		return m_identifier == object.m_identifier;
	}

	const String& RenderBase::GetName() const {
		return m_name;
	}

	IContext* RenderBase::GetContext() const {
		return m_context;
	}
}