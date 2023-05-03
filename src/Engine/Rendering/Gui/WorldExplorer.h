#ifndef WORLDEXPLORER
#define WORLDEXPLORER

#include "Engine/Core/Render/Gui/GuiWidget.h"

namespace Engine {
	class WorldExplorer : public GuiWidget {
	public:
		WorldExplorer(const String& name, const String& tag = "WorldExplorer");
		virtual ~WorldExplorer() = default;
	};
}

#endif // !WORLDEXPLORER
