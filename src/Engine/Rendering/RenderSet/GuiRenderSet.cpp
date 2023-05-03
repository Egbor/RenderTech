#include "Engine/Rendering/RenderSet/GuiRenderSet.h"

#include "Engine/Rendering/RenderPass/DevGuiRenderPass.h"

namespace Engine {
	GuiRenderSet::GuiRenderSet(GuiContext* context, Texture2D* target) 
		: m_rootLayout(new MainWindow(context)) {
		AddRenderPass<DevGuiRenderPass, GuiContext*, GuiLayout*, Texture2D*>(context, m_rootLayout, target);
	}

	GuiRenderSet::~GuiRenderSet() {
		DELETE_OBJECT(m_rootLayout);
	}

	Texture2D* GuiRenderSet::GetViewportTarget() const {
		return m_rootLayout->GetViewportTarget();
	}
}