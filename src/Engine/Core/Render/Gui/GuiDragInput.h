#ifndef GUIDRAGINPUT
#define GUIDRAGINPUT

#include "Engine/Core/Render/Gui/GuiLayout.h"

namespace Engine {
	class GuiDragInput : public GuiLayout {
	private:
		String m_label;
		Float* m_data;

	public:
		GuiDragInput(const String& tag, const String& label);
		virtual ~GuiDragInput() = default;

		void SetData(Float* data);

	private:
		void Render(void* layoutData) override;
	};
}

#endif // !GUIDRAGINPUT
