#ifndef DEVGUIRENDERPASS
#define DEVGUIRENDERPASS

#include "Engine/Rendering/RenderPass/RenderPass.h"

#include "Engine/Core/Render/Gui/GuiContext.h"
#include "Engine/Core/Render/Gui/GuiLayout.h"
#include "Engine/Core/Render/Api/RenderBinder.h"

namespace Engine {
	class DevGuiRenderPass : public RenderPass {
	private:
		GuiContext* m_devguiContext;
		GuiLayout* m_rootLayout;

		RenderBinderBase* m_targetBinder;

	public:
		DevGuiRenderPass(GuiContext* devguiContext, GuiLayout* root, Texture2D* target);
		virtual ~DevGuiRenderPass() = default;

		void Render(RenderPass* prev) override;

	protected:
		void AttachRenderTarget();
		void DetachRenderTarget();
	};
}

#endif // !DEVGUIRENDERPASS

