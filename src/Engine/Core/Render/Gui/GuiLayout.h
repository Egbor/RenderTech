#ifndef GUILAYOUT
#define GUILAYOUT

#include "Engine/EngineDef.h"
#include "Engine/Tools/Event.h"

#define DELETE_LAYOUT(layout)				\
	RemoveChildLayout((layout)->GetTag());	\
	DELETE_OBJECT((layout));

namespace Engine {
	class GuiLayout {
	private:
		String m_layoutTag;

		Map<String, GuiLayout*> m_childLayouts;

	protected:
		Event<GuiLayout*> m_eventOnClick;

	public:
		GuiLayout(const String& tag);
		virtual ~GuiLayout() = default;

		virtual void Render();
		String GetTag() const;

		void AddOnClickEvent(EventBase<GuiLayout*>& callback);
		void RemoveOnClickEvent(EventBase<GuiLayout*>& callback);

	protected:
		void RenderChildLayouts();
		void AddChildLayout(GuiLayout* layout);
		GuiLayout* GetChildLayout(const String& tag);
		void RemoveChildLayout(const String& tag);

		void InvokeOnClickEvent();
	};
}

#endif // !GUILAYOUT
