#ifndef INPUT_H
#define INPUT_H

#include "Engine/Core/System/Platform/Common/InputConfig.h"
#include "Engine/Core/Utils/Event.h"

namespace Engine {
	struct InputValue {
		InputValue() = default;

		InputValue(Float value) 
			: axisValue(value) {

		}

		InputValue(InputEvent value) 
			: actionValue(value) {

		}

		union {
			Float axisValue;
			InputEvent actionValue;
		};
	};

	class Input {
	public:
		Input() = default;
		virtual ~Input() = default;

		void AddAction(const String& tag, EventBase<>& callback);
		void AddAxis(const String& tag, EventBase<Float>& callback);

		void NotifyAboutActionTriggering(const InputConfig* config);
		void NotifyAboutAxisTriggering(const InputConfig* config, Float value);

	protected:
		void SetValueInKeymap(InputKey key, InputValue value);

	private:
		Map<InputKey, InputValue> m_keymap;

		Map<String, Event<>> m_actions;
		Map<String, Event<Float>> m_axis;
	};
}

#endif // !INPUT_H
