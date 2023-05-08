#ifndef GUICOLORPICKER
#define GUICOLORPICKER

#include "Engine/Core/Render/Gui/GuiLayout.h"

namespace Engine {
	class GuiColorPicker : public GuiLayout {
	private:
		String m_label;
		Float* m_data;

	public:
		GuiColorPicker(const String& tag, const String& label);
		virtual ~GuiColorPicker() = default;

		void SetData(Float* data);

	private:
		void Render(void* layoutData) override;
	};
}

#endif // !GUICOLORPICKER
