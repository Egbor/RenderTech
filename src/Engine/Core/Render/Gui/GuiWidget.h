#ifndef GUIWIDGET
#define GUIWIDGET

#include "Engine/Core/Render/Gui/GuiLayout.h"

namespace Engine {
	class GuiWidget : public GuiLayout {
	private:
		String m_name;

		Int32 m_width;
		Int32 m_height;

		Event<GuiLayout*, Int32, Int32> m_eventOnResize;

	public:
		GuiWidget(const String& tag, const String& name);
		virtual ~GuiWidget() = default;

		String GetName() const;

		Int32 GetWidth() const;
		Int32 GetHeight() const;

		void AddOnResizeEvent(EventBase<GuiLayout*, Int32, Int32>& callback);
		void RemoveResizeEvent(EventBase<GuiLayout*, Int32, Int32>& callback);

	private:
		void Render(void* layoutData) override;
	};
}

#endif // !GUIWIDGET
