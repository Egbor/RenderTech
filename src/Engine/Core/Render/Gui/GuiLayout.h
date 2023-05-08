#ifndef GUILAYOUT
#define GUILAYOUT

#include "Engine/EngineDef.h"
#include "Engine/Tools/Event.h"

#define DELETE_LAYOUT(layout)					\
	if (layout != nullptr) {					\
		RemoveChildLayout((layout)->GetTag());	\
		DELETE_OBJECT((layout));				\
	}

namespace Engine {
	class GuiLayout {
	private:
		String m_layoutTag;
		bool m_isVisible;

		GuiLayout* m_parentLayout;
		Array<GuiLayout*> m_childLayouts;

	protected:
		Event<GuiLayout*> m_eventOnClick;

	public:
		GuiLayout(const String& tag);
		virtual ~GuiLayout();

		virtual void Render(void* layoutData = nullptr);

		String GetTag() const;
		GuiLayout* GetParent() const;

		void SetVisible(bool value);
		bool GetVisible() const;

		bool AddChildLayout(GuiLayout* layout);
		bool RemoveChildLayout(const String& tag, GuiLayout** outLayout = nullptr);

		void AddOnClickEvent(EventBase<GuiLayout*>& callback);
		void RemoveOnClickEvent(EventBase<GuiLayout*>& callback);

	protected:
		void RenderChildLayouts(void* layoutData);

	private:
		void Register(GuiLayout* parent);
		void UnRegister();
	};
}

#endif // !GUILAYOUT
