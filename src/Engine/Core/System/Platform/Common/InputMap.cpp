#include "Engine/Core/System/Platform/Common/InputMap.h"

#include <mutex>

namespace Engine {
	std::mutex mtxInput;

	InputValue& InputMap::operator[](InputKey key) {
		std::lock_guard lock(mtxInput);
		return m_keymap[key];
	}
}