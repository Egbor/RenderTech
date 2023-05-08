#include "Engine/Rendering/Gui/Properties.h"

#include "Engine/Object/Component/LightComponent.h"

namespace Engine {
	Properties::Properties(const String& name, const String& tag) 
		: GuiWidget(tag, name) {
		m_position = new GuiDragInput("Position", "Position");
		m_rotation = new GuiDragInput("Rotation", "Rotation");
		m_scale = new GuiDragInput("Scale", "Scale");
		m_color = new GuiColorPicker("Color", "Color");

		AddChildLayout(m_position);
		AddChildLayout(m_rotation);
		AddChildLayout(m_scale);
	}

	Properties::~Properties() {
		DELETE_LAYOUT(m_color);
		DELETE_LAYOUT(m_scale);
		DELETE_LAYOUT(m_rotation);
		DELETE_LAYOUT(m_position);
	}

	void Properties::SetComponent(SceneComponent* component) {
		if (component == nullptr) {
			return;
		}

		m_position->SetData(reinterpret_cast<Float*>(&component->position));
		m_rotation->SetData(reinterpret_cast<Float*>(&component->rotation));
		m_scale->SetData(reinterpret_cast<Float*>(&component->scale));

		if (component->Is(LightComponent::TypeIdClass())) {
			LightComponent* temp = component->As<LightComponent>();
			m_color->SetData(reinterpret_cast<Float*>(&temp->color));
			AddChildLayout(m_color);
		} else {
			RemoveChildLayout(m_color->GetTag());
		}
	}
}