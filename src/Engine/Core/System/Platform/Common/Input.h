#ifndef INPUT_H
#define INPUT_H

#include "Engine/Core/System/Platform/Common/InputConfig.h"
#include "Engine/Core/System/Platform/Common/InputMap.h"
#include "Engine/Core/Utils/Event.h"

namespace Engine {
	class Input {
	public:
		Input() = default;
		virtual ~Input() = default;

		void AddAction(const String& tag, EventBase<>& callback);
		void AddAxis(const String& tag, EventBase<Float>& callback);

		void NotifyAboutActionTriggering(const InputConfig* config);
		void NotifyAboutAxisTriggering(const InputConfig* config);

		virtual void Refresh();

	protected:
		void SetValueInKeymap(InputKey key, InputValue value);

	private:
		InputMap m_keymap;

		Map<String, Event<>> m_actions;
		Map<String, Event<Float>> m_axis;
	};
}

#endif // !INPUT_H
