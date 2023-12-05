#ifndef INPUT_COMPONENT_H
#define INPUT_COMPONENT_H

#include "Engine/Object/Component/EntityComponent.h"

#include "Engine/Core/System/Platform/Common/Input.h"
#include "Engine/Core/Utils/Event.h"

namespace Engine {
	CLASSTYPE(InputComponent)
	class InputComponent : public EntityComponent {
		GENERATE_BODY(InputComponent, EntityComponent)

	private:
		InputConfig m_config;

	public:
		InputComponent(const ObjectArgument& argument);
		virtual ~InputComponent() = default;

		template<class TOwner>
		void BindAction(const String& tag, TOwner* owner, typename Method<TOwner>::TMethodPtr method) {
			Input* input = Core::GetInstance()->GetInput();
			input->AddAction(tag, Delegate<TOwner>::Allocate(owner, method));
		}

		template<class TOwner>
		void BindAxis(const String& tag, TOwner* owner, typename Method<TOwner, Float>::TMethodPtr method) {
			Input* input = Core::GetInstance()->GetInput();
			input->AddAxis(tag, Delegate<TOwner, Float>::Allocate(owner, method));
		}

		void UpdateInputs(Float deltaTime);
	};
}

#endif // !INPUT_COMPONENT_H
