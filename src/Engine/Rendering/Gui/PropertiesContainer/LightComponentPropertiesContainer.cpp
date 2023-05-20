#include "Engine/Rendering/Gui/PropertiesContainer/LightComponentPropertiesContainer.h"

#include "Engine/Object/Component/LightComponent.h"

namespace Engine {
	LightComponentPropertiesContainer::LightComponentPropertiesContainer() {
		m_color = new GuiColorPicker("LightColor", "Light color");
	}

	LightComponentPropertiesContainer::~LightComponentPropertiesContainer() {
		DELETE_OBJECT(m_color);
	}

	void LightComponentPropertiesContainer::Attach(GuiLayout* layout, SceneComponent* component) {
		if (component->Is(LightComponent::TypeIdClass())) {
			LightComponent* temp = component->As<LightComponent>();

			m_color->SetData(reinterpret_cast<Float*>(&temp->color));
			layout->AddChildLayout(m_color);
		} else {
			Detach(layout);
		}
	}

	void LightComponentPropertiesContainer::Detach(GuiLayout* layout) {
		layout->RemoveChildLayout(m_color->GetTag());
	}
}