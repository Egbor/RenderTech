#include "Engine/Core/System/Platform/Common/Input.h"

namespace Engine{
	void Input::AddAction(const String& tag, EventBase<>& callback) {
		m_actions[tag] += callback;
	}

	void Input::AddAxis(const String& tag, EventBase<Float>& callback) {
		m_axis[tag] += callback;
	}

	void Input::NotifyAboutActionTriggering(const InputConfig* config) {
		for (auto action = m_actions.begin(); action != m_actions.end(); action++) {
			const Array<InputBinding<InputEvent>>& bindings = config->GetActions(action->first);
			for (auto binding = bindings.begin(); binding != bindings.end(); binding++) {
				if (m_keymap[binding->inputKey].actionValue == binding->inputValue) {
					action->second();
				}
			}
		}
	}

	void Input::NotifyAboutAxisTriggering(const InputConfig* config) {
		for (auto axis = m_axis.begin(); axis != m_axis.end(); axis++) {
			const Array<InputBinding<Float>>& bindings = config->GetAxis(axis->first);
			for (auto binding = bindings.begin(); binding != bindings.end(); binding++) {
				axis->second(binding->inputValue * m_keymap[binding->inputKey].axisValue);
			}
		}
	}

	void Input::Refresh() {

	}

	void Input::SetValueInKeymap(InputKey key, InputValue value) {
		m_keymap[key] = value;
	}
}