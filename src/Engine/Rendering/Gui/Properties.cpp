#include "Engine/Rendering/Gui/Properties.h"

namespace Engine {
	Properties::Properties(const String& name, const String& tag) 
		: GuiWidget(tag, name) {
		m_position = new GuiDragInput("Position", "Position");
		m_rotation = new GuiDragInput("Rotation", "Rotation");
		m_scale = new GuiDragInput("Scale", "Scale");

		AddChildLayout(m_position);
		AddChildLayout(m_rotation);
		AddChildLayout(m_scale);
	}

	Properties::~Properties() {
		DELETE_LAYOUT(m_scale);
		DELETE_LAYOUT(m_rotation);
		DELETE_LAYOUT(m_position);
	}

	void Properties::SetEntity(Entity* entity) {
		if (entity == nullptr) {
			return;
		}

		m_position->SetData(reinterpret_cast<Float*>(&entity->GetRootComponent()->position));
		m_rotation->SetData(reinterpret_cast<Float*>(&entity->GetRootComponent()->rotation));
		m_scale->SetData(reinterpret_cast<Float*>(&entity->GetRootComponent()->scale));
	}
}