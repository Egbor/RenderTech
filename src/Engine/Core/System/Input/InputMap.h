#ifndef INPUTMAP_H
#define INPUTMAP_H

#include "Engine/EngineDef.h"

namespace Engine {
    class InputMap {
    private:
        Map<InputKey, Float> m_generalKeyMap;

    public:
        InputMap() = default;
        virtual ~InputMap() = default;

        void SetKeyInteraction(InputKey key, Float inValue);
        bool HasKeyInteraction(InputKey key, Float& outValue);

        void RegisterKey(InputKey key);
        void UnRegisterKey(InputKey key);
    };
}

#endif // INPUTMAP_H
