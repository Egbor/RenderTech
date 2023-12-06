#include "Engine.h"

#include "Engine/Rendering/Engine/RenderPass/BaseRenderPass.h"
#include "Engine/Rendering/Engine/RenderPass/LightRenderPass.h"

#include "Engine/Core/System/Platform/Common/Input.h"

namespace Engine {
	EngineClass::EngineClass(IWindow* window, IContext* context) 
		: m_time(new Time()), m_gameThread(new EngineThread()), m_renderThread(new EngineThread) {
		Core::GetInstance()->Initialize(window, context);

		m_passContext = new RenderPassContext(context->QueryPipeline(), window->GetWidth(), window->GetHeight());
		m_passContext->Append(new BaseRenderPass(), context->QuerySwapChain());
		//m_passContext->Append(new LightRenderPass(), context->QuerySwapChain());
	}

	EngineClass::~EngineClass() {
		DELETE_OBJECT(m_renderThread);
		DELETE_OBJECT(m_gameThread);

		DELETE_OBJECT(m_time);
		DELETE_OBJECT(m_passContext);
	}

	void EngineClass::Run(World* world) {
		EngineThreadSync sync(2);
		IWindow* window = Core::GetInstance()->GetWindow();

		m_time->Start();
		m_gameThread->Start(sync, Delegate<EngineClass>::Allocate(this, &EngineClass::RunGameThread));
		m_renderThread->Start(sync, Delegate<EngineClass>::Allocate(this, &EngineClass::RunRenderThread));

		while (!window->HasQuit()) {
			sync.Allow();
		}

		m_gameThread->Wait();
		m_renderThread->Wait();
	}

	void EngineClass::RunGameThread() {
		m_time->Tick();
	}

	void EngineClass::RunRenderThread() {
		m_passContext->Render(Core::GetInstance()->GetContext());
	}

	//void EngineClass::OnWorldStart(Entity* entity) {
	//	entity->OnStart();
	//}

	//void EngineClass::OnWorldUpdate(Entity* entity) {
	//	entity->OnUpdate(m_time->DeltaTime());

	//	Array<SceneComponent*> components;
	//	entity->GetRootComponent()->GetChildrenComponents(components);
	//	m_passContext->Process(components);
	//}
}
