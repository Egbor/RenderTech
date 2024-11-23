#ifndef INPUT_COMPONENT_H
#define INPUT_COMPONENT_H

#include "Engine/Object/Component/EntityComponent.h"

#include "Engine/Core/System/Platform/Common/Input.h"
#include "Engine/Core/Core.h"
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
		void BindAction(const String& tag, TOwner* owner, typename Method<TOwner, void()>::TMethodPtr method) {
			Input* input = Core::GetInstance()->GetInput();
			input->AddAction(tag, Callable<void()>::AllocateDelegate(owner, method));
		}

		template<class TOwner>
		void BindAxis(const String& tag, TOwner* owner, typename Method<TOwner, void(Float)>::TMethodPtr method) {
			Input* input = Core::GetInstance()->GetInput();
			input->AddAxis(tag, Callable<void(Float)>::AllocateDelegate(owner, method));
		}

		void UpdateInputs();
	};
}

#endif // !INPUT_COMPONENT_H
