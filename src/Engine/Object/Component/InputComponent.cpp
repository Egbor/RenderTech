#include "Engine/Object/Component/InputComponent.h"

namespace Engine {
	GENERATE_INSTANTIATION(InputComponent)

	InputComponent::InputComponent(const ObjectArgument& argument) 
		: EntityComponent(argument), m_config("config/player.cfg") {

	}

	void InputComponent::UpdateInputs() {
		Input* input = Core::GetInstance()->GetInput();
		input->NotifyAboutActionTriggering(&m_config);
		input->NotifyAboutAxisTriggering(&m_config);
	}
}