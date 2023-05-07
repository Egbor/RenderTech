#ifndef WORLDEXPLORER
#define WORLDEXPLORER

#include "Engine/Core/Render/Gui/GuiWidget.h"
#include "Engine/Core/Render/Gui/GuiTreeNode.h"
#include "Engine/Object/Entity/Entity.h"

namespace Engine {
	class WorldExplorer : public GuiWidget {
	private:
		GuiTree* m_root;

	public:
		WorldExplorer(const String& name, const String& tag = "WorldExplorer");
		virtual ~WorldExplorer();

		void AddEntityToExplorer(Entity* entity);
		void RemoveEntityFromExplorer(Entity* entity);

		void AddOnEntitySelectedEvent(EventBase<GuiLayout*, GuiTreeNode*>& callback);
		void RemoveOnEntitySelectedEvent(EventBase<GuiLayout*, GuiTreeNode*>& callback);
	};
}

#endif // !WORLDEXPLORER
