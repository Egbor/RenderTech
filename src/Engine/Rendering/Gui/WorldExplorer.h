#ifndef WORLDEXPLORER
#define WORLDEXPLORER

#include "Engine/Core/Render/Gui/GuiWidget.h"
#include "Engine/Core/Render/Gui/GuiTreeNode.h"
#include "Engine/Object/Entity/Entity.h"

namespace Engine {
	class WorldExplorer : public GuiWidget {
	private:
		GuiTreeNode* m_root;

	public:
		WorldExplorer(const String& name, const String& tag = "WorldExplorer");
		virtual ~WorldExplorer();

		void AddEntityToExplorer(Entity* entity);
		void RemoveEntityFromExplorer(Entity* entity);
	};
}

#endif // !WORLDEXPLORER
