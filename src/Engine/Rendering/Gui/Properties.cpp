#include "Engine/Rendering/Gui/Properties.h"

#include "Engine/Rendering/Gui/PropertiesContainer/MeshComponentPropertiesContainer.h"
#include "Engine/Rendering/Gui/PropertiesContainer/LightComponentPropertiesContainer.h"

namespace Engine {
	Properties::Properties(const String& name, const String& tag) 
		: GuiWidget(tag, name), m_containers() {
		m_position = new GuiDragInput("Position", "Position");
		m_rotation = new GuiDragInput("Rotation", "Rotation");
		m_scale = new GuiDragInput("Scale", "Scale");

		m_containers.push_back(new MeshComponentPropertiesContainer());
		m_containers.push_back(new LightComponentPropertiesContainer());

		AddChildLayout(m_position);
		AddChildLayout(m_rotation);
		AddChildLayout(m_scale);
	}

	Properties::~Properties() {
		for (auto it = m_containers.begin(); it != m_containers.end(); it++) {
			(*it)->Detach(this);
			DELETE_OBJECT(*it);
		}

		DELETE_LAYOUT(m_scale);
		DELETE_LAYOUT(m_rotation);
		DELETE_LAYOUT(m_position);
	}

	void Properties::SetComponent(SceneComponent* component) {
		m_position->SetData(reinterpret_cast<Float*>(&component->position));
		m_rotation->SetData(reinterpret_cast<Float*>(&component->rotation));
		m_scale->SetData(reinterpret_cast<Float*>(&component->scale));

		for (auto it = m_containers.begin(); it != m_containers.end(); it++) {
			(*it)->Attach(this, component);
		}
	}
}