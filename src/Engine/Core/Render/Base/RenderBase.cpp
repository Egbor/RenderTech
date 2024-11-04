#include "Engine/Core/Render/Base/RenderBase.h"

namespace Engine {
	RenderBase::RenderBase(ResourceIdentifier identifier, IContext* context) 
		: m_identifier(identifier), m_context(context) {

	}

	bool RenderBase::Is(ResourceIdentifier identifier) const {
		return m_identifier == identifier;
	}

	bool RenderBase::Is(const RenderBase& object) const {
		return m_identifier == object.m_identifier;
	}

	IContext* RenderBase::GetContext() const {
		return m_context;
	}
}