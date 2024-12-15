#ifndef RENDERENGINE_H
#define RENDERENGINE_H

#include "EngineThread.h"
#include "Engine/EngineDef.h"

#include "Engine/Rendering/Engine/HighRenderContext.h"
#include "Engine/Core/System/Time/Time.h"
#include "Engine/Object/World/World.h"

#include "Engine/Core/Runtime/Runtime.h"

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

        Time* m_time;
        World* m_world;

        AbstractHighRenderContext* m_context;
        EngineThreadPool* m_threadpool;
    };

    class EngineRuntime : public Runtime {
    public:
        EngineRuntime(IWindow* window, IContext* context);
        EngineRuntime(const EngineRuntime&) = delete;
        EngineRuntime(EngineRuntime&&) = delete;
        ~EngineRuntime();

        EngineRuntime& operator=(const EngineRuntime&) = delete;
        EngineRuntime& operator=(EngineRuntime&&) = delete;

        void Init();
        void Start() override;

    private:
        void UpdateLoop(Float deltaTime);
        void RenderLoop(Float deltaTime);

        AbstractHighRenderContext* m_context;
    };
}

#endif // RENDERENGINE_H
