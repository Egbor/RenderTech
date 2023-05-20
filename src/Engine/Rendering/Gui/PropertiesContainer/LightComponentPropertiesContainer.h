#ifndef LIGHTCOMPONENTPROPERTIESCONTAINER
#define LIGHTCOMPONENTPROPERTIESCONTAINER

#include "Engine/Rendering/Gui/PropertiesContainer/PropertiesContainer.h"

#include "Engine/Core/Render/Gui/GuiColorPicker.h"

namespace Engine {
	class LightComponentPropertiesContainer : public PropertiesContainer {
	private:
		GuiColorPicker* m_color;

	public:
		LightComponentPropertiesContainer();
		virtual ~LightComponentPropertiesContainer();

		void Attach(GuiLayout* layout, SceneComponent* component) override;
		void Detach(GuiLayout* layout) override;
	};
}

#endif // !LIGHTCOMPONENTPROPERTIESCONTAINER
