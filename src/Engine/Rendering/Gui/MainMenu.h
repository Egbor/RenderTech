#ifndef MAINMENU
#define MAINMENU

#include "Engine/Core/Render/Gui/GuiMenuBar.h"

namespace Engine {
	class MainMenu : public GuiMenuBar {
	private:
		Event<const String&> m_eventOnAddObjectEvent;

	public:
		MainMenu(const String& tag = "MainMenu");
		virtual ~MainMenu() = default;

		void AddOnAddObjectEvent(EventBase<const String&>& callback);
		void RemoveOnAddObjectEvent(EventBase<const String&>& callback);

	private:
		void OnClickCloseMenuItem(GuiLayout* owner);
		void OnClickAddObject(GuiLayout* owner);
	};
}

#endif // !MAINMENU
