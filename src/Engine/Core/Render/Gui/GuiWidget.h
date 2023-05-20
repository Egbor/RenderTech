#ifndef GUIWIDGET
#define GUIWIDGET

#include "Engine/Core/Render/Gui/GuiLayout.h"

namespace Engine {
	class GuiWidget : public GuiLayout {
	private:
		String m_name;

		bool m_open;
		Int32 m_width;
		Int32 m_height;

		Event<GuiLayout*> m_eventOnClose;
		Event<GuiLayout*> m_eventOnShow;
		Event<GuiLayout*, Int32, Int32> m_eventOnResize;

	public:
		GuiWidget(const String& tag, const String& name);
		virtual ~GuiWidget() = default;

		String GetName() const;

		Int32 GetWidth() const;
		Int32 GetHeight() const;

		void Show();

		void AddOnCloseEvent(EventBase<GuiLayout*>& callback);
		void AddOnShowEvent(EventBase<GuiLayout*>& callback);
		void AddOnResizeEvent(EventBase<GuiLayout*, Int32, Int32>& callback);
		void RemoveOnCloseEvent(EventBase<GuiLayout*>& callback);
		void RemoveOnShowEvent(EventBase<GuiLayout*>& callback);
		void RemoveResizeEvent(EventBase<GuiLayout*, Int32, Int32>& callback);

	private:
		void Render(void* layoutData) override;

		void HandleOnClickEvent();
		void HandleOnResizeEvent();
		void HandleOnCloseEvent();
	};
}

#endif // !GUIWIDGET
