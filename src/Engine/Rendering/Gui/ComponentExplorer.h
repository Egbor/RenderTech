#ifndef COMPONENTEXPLORER
#define COMPONENTEXPLORER

#include "Engine/Core/Render/Gui/GuiWidget.h"
#include "Engine/Core/Render/Gui/GuiTreeNode.h"
#include "Engine/Object/Entity/Entity.h"

namespace Engine {
	class ComponentExplorer : public GuiWidget {
	private:
		GuiTree* m_root;
		String m_rootTag;
	public:
		ComponentExplorer(const String& name, const String& tag = "ComponentExplorer");
		virtual ~ComponentExplorer();

		void SetComponentsToExplorer(Entity* entity);

		void AddOnComponentSelectedEvent(EventBase<GuiLayout*, GuiTree*>& callback);
		void RemoveOnComponentSelectedEvent(EventBase<GuiLayout*, GuiTree*>& callback);
	};
}

#endif // !COMPONENTEXPLORER

