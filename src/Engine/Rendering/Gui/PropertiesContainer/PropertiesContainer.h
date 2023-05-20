#ifndef PROPERTIESCONTAINER
#define PROPERTIESCONTAINER

#include "Engine/Core/Render/Gui/GuiLayout.h"
#include "Engine/Object/Component/SceneComponent.h"

namespace Engine {
	class PropertiesContainer {
	public:
		PropertiesContainer() = default;
		virtual ~PropertiesContainer() = default;

		virtual void Attach(GuiLayout* layout, SceneComponent* component) = 0;
		virtual void Detach(GuiLayout* layout) = 0;
	};
}

#endif // !PROPERTIESCONTAINER
