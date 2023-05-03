#include "Engine/Rendering/RenderSet/GuiRenderSet.h"

#include "Engine/Rendering/RenderPass/DevGuiRenderPass.h"

namespace Engine {
	GuiRenderSet::GuiRenderSet(GuiContext* context, Texture2D* target) 
		: RenderSet(target), m_rootLayout(new MainWindow()) {
		m_viewportTarget = context->GetContext()->CreateTexture2D(tagTargetTexture, { (UInt32)target->GetWidth(), (UInt32)target->GetHeight(), target->GetFormat() });
		m_rootLayout->SetViewportTexture(context, m_viewportTarget);

		AddRenderPass<DevGuiRenderPass, GuiContext*, GuiLayout*, Texture2D*>(context, m_rootLayout, target);
	}

	GuiRenderSet::~GuiRenderSet() {
		DELETE_OBJECT(m_rootLayout);
		DELETE_OBJECT(m_viewportTarget);
	}

	Texture2D* GuiRenderSet::GetViewportTarget() const {
		return m_viewportTarget;
	}

	void GuiRenderSet::AddViewportResizeEvent(EventBase<Int32, Int32>& callback) {
		m_rootLayout->AddViewportResizeEvent(callback);
	}

	void GuiRenderSet::RemoveViewportResizeEvent(EventBase<Int32, Int32>& callback) {
		m_rootLayout->RemoveViewportResizeEvent(callback);
	}
}