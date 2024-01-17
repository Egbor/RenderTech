#ifndef INPUT_MAP_H
#define INPUT_MAP_H

#include "Engine/Core/System/Platform/Common/InputDef.h"

namespace Engine {
	struct InputMap {
	public:
		InputValue& operator[](InputKey key);

	private:
		Map<InputKey, InputValue> m_keymap;
	};
}

#endif // !INPUT_MAP_H
