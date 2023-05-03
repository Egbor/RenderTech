#include "Engine/Rendering/Utils/TargetMap.h"

namespace Engine {
	TargetMap::TargetMap(Context* context) 
		: m_context(context), m_targets(RENDER_OUTPUT_COUNT) {

	}

	TargetMap::~TargetMap() {
		DELETE_ARRAY_OF_OBJECTS(m_targets);
	}

	void TargetMap::AddTarget(RenderOutput slot, Texture2D* texture) {
		RemoveTarget(slot);
		m_targets[static_cast<Int32>(slot)] = m_context->CreateTarget(texture->GetTag(), texture);
	}

	void TargetMap::AddTarget(RenderOutput slot, TextureFormat format, UInt32 width, UInt32 height) {
		TextureInfo info { width, height, format };
		String tag = (slot == RenderOutput::RO_DEPTH) ? tagDepthStencilTargetTexture : tagTargetTexture;

		Texture2D* texture = m_context->CreateTexture2D(tag, info);
		AddTarget(slot, texture);
		DELETE_OBJECT(texture);
	}

	void TargetMap::AddTarget(RenderOutput slot, Target* target) {
		Texture2D* texture = m_context->Copy(target->GetTexture2D());
		AddTarget(slot, texture);
		DELETE_OBJECT(texture);
	}

	void TargetMap::RemoveTarget(RenderOutput slot) {
		DELETE_OBJECT(m_targets[static_cast<Int32>(slot)]);
	}

	Target* TargetMap::GetTarget(RenderOutput slot) const {
		return m_targets[static_cast<Int32>(slot)];
	}
}