#ifndef GUIBUTTON
#define GUIBUTTON

#include "Engine/Core/Render/Gui/GuiLayout.h"

namespace Engine {
	class GuiButton : public GuiLayout {
	private:
		String m_label;

	public:
		GuiButton(const String& tag, const String& label);
		virtual ~GuiButton() = default;

	private:
		void Render(void* layoutData) override;
	};
}

#endif // !GUIBUTTON
