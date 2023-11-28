#ifndef RENDERENGINE_H
#define RENDERENGINE_H

#include "Engine/EngineDef.h"
#include "Engine/Rendering/Engine/RenderPass/RenderPassContext.h"

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
        RenderPassContext* m_passContext;
    };
}

#endif // RENDERENGINE_H
