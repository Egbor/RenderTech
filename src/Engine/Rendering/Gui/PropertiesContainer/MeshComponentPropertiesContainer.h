#ifndef MESHCOMPONENTPROPERTIESCONTAINER
#define MESHCOMPONENTPROPERTIESCONTAINER

#include "Engine/Rendering/Gui/PropertiesContainer/PropertiesContainer.h"

#include "Engine/Core/Render/Gui/GuiButton.h"

namespace Engine {
	class MeshComponentPropertiesContainer : public PropertiesContainer {
	private:
		GuiButton* m_meshButton;

	public:
		MeshComponentPropertiesContainer();
		virtual ~MeshComponentPropertiesContainer();

		void Attach(GuiLayout* layout, SceneComponent* component) override;
		void Detach(GuiLayout* layout) override;

	private:
		void OnClickMeshButton(GuiLayout* owner);
	};
}

#endif // !MESHCOMPONENTPROPERTIESCONTAINER
