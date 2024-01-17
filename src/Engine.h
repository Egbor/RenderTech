#ifndef RENDERENGINE_H
#define RENDERENGINE_H

#include "EngineThread.h"
#include "Engine/EngineDef.h"

#include "Engine/Rendering/Engine/RenderPass/RenderPassContext.h"
#include "Engine/Core/System/Time/Time.h"
#include "Engine/Object/World/World.h"

namespace Engine {
    class EngineClass {
    public:
        EngineClass(IWindow* window, IContext* context);
        virtual ~EngineClass();

        void Run();
        World* GetWorld() const;

    private:
        void SyncEntry();
        void GameThreadEntry();
        void RenderThreadEntry();

        void InvokeEntitiesRenderUpdate(Entity* entity);

        Time* m_time;
        World* m_world;
        RenderPassContext* m_passContext;

        EngineThreadPool* m_threadpool;
    };
}

#endif // RENDERENGINE_H
