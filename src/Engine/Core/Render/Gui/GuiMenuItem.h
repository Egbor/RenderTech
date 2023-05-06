#ifndef GUIMENUITEM
#define GUIMENUITEM

#include "Engine/Core/Render/Gui/GuiLayout.h"

namespace Engine {
	class GuiMenuItem : public GuiLayout {
	private:
		String m_label;

	public:
		GuiMenuItem(const String& tag, const String& label);
		virtual ~GuiMenuItem() = default;

		void Render(void* layoutData) override;

		String GetLabel() const;
	};
}

#endif // !GUIMENUITEM
