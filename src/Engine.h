#ifndef RENDERENGINE_H
#define RENDERENGINE_H

#include "Engine/EngineDef.h"
#include "Engine/Core/Render/Api/Interface/IContext.h"
#include "Engine/Core/System/Platform/Interface/IWindow.h"

#include "Engine/Core/System/Time/Time.h"
#include "Engine/Object/World/World.h"

namespace Engine {
    class EngineClass {
    public:
        EngineClass(IWindow* window, IContext* context);
        virtual ~EngineClass();

        void Run(World* world);

        void OnWorldStart(Entity* entity);
        void OnWorldUpdate(Entity* entity);

    private:
        Time* m_time;
    };
}

#endif // RENDERENGINE_H
