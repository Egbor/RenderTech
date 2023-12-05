#ifndef INPUT_CONFIG_H
#define INPUT_CONFIG_H

#include "Engine/EngineDef.h"
#include "Engine/Core/System/Platform/Common/InputDef.h"

namespace Engine {
	class InputConfig {
	public:
		InputConfig(const String& filename);
		virtual ~InputConfig() = default;

		const Array<InputBinding<InputEvent>>& GetActions(const String& tag) const;
		const Array<InputBinding<Float>>& GetAxis(const String& tag) const;

	private:
		Map<String, Array<InputBinding<InputEvent>>> m_actions;
		Map<String, Array<InputBinding<Float>>> m_axis;
	};
}

#endif // !INPUT_CONFIG_H
