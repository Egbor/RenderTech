#ifndef INPUT_H
#define INPUT_H

#include "Engine/Tools/Event.h"

#include "Engine/Core/System/Input/InputMap.h"
#include "Engine/Core/System/Input/InputSettings.h"

namespace Engine {
    class Input {
    private:
        InputMap m_inputMap;
        InputSettings m_inputSettings;

    public:
        Input();
        virtual ~Input() = default;

        void SetupAction(const String& tag, InputKey key, InputEvent event);
        void SetupAxis(const String& tag, InputKey key, Float value);

        void HandleAction(const String& tag, Event<>& event);
        void HandleAxis(const String& tag, Event<Float>& event);

    protected:
        InputMap& GetInputMap();
        InputSettings& GetInputSettings();
    };
}

#endif // INPUT_H
