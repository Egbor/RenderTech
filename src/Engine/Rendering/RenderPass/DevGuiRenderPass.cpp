#include "Engine/Rendering/RenderPass/DevGuiRenderPass.h"

namespace Engine {
	DevGuiRenderPass::DevGuiRenderPass(GuiContext* devguiContext, GuiLayout* root, Texture2D* target)
		: RenderPass(devguiContext->GetContext()), m_devguiContext(devguiContext),
		m_rootLayout(root), m_targetBinder(devguiContext->GetContext()->CreateBinder(tagREALTIME_TARGET_BINDER)) {
		AddTarget(RenderOutput::RO_TARGET0, target);
		AttachRenderTarget();
	}

	void DevGuiRenderPass::Render(RenderPass* prev) {
		TargetClear clear;
		memset(&clear, 0, sizeof(clear));
		ClearTarget(RenderOutput::RO_TARGET0, clear);

		m_targetBinder->Bind();
		m_devguiContext->PrepareLayout();
		m_rootLayout->Render();
		m_devguiContext->Draw();
		m_targetBinder->UnBind();
	}

	void DevGuiRenderPass::AttachRenderTarget() {
		m_targetBinder->AttachRenderItem(static_cast<Int32>(RenderOutput::RO_TARGET0), GetTarget(RenderOutput::RO_TARGET0));
	}

	void DevGuiRenderPass::DetachRenderTarget() {
		m_targetBinder->DetachRenderItem(static_cast<Int32>(RenderOutput::RO_TARGET0));
	}
}