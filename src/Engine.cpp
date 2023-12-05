#include "Engine.h"

#include "Engine/Rendering/Engine/RenderPass/BaseRenderPass.h"
#include "Engine/Rendering/Engine/RenderPass/LightRenderPass.h"

#include "Engine/Core/System/Platform/Common/Input.h"

namespace Engine {
	EngineClass::EngineClass(IWindow* window, IContext* context) 
		: m_time(new Time()) {
		Core::GetInstance()->Initialize(window, context);

		m_passContext = new RenderPassContext(context->QueryPipeline(), window->GetWidth(), window->GetHeight());
		m_passContext->Append(new BaseRenderPass(), context->QuerySwapChain());
		//m_passContext->Append(new LightRenderPass(), context->QuerySwapChain());
	}

	EngineClass::~EngineClass() {
		DELETE_OBJECT(m_time);
		DELETE_OBJECT(m_passContext);
	}

	void EngineClass::Run(World* world) {
		static EventBase<Entity*>& onStart = Delegate<EngineClass, Entity*>::Allocate(this, &EngineClass::OnWorldStart);
		static EventBase<Entity*>& onUpdate = Delegate<EngineClass, Entity*>::Allocate(this, &EngineClass::OnWorldUpdate);

		IWindow* window = Core::GetInstance()->GetWindow();
		IContext* context = Core::GetInstance()->GetContext();

		m_time->Start();

		world->ForEachEntity(onStart);
		while (!window->HasQuit()) {
			world->ForEachEntity(onUpdate);

			m_passContext->Render(context);
			m_time->Tick();
		}
	}

	void EngineClass::OnWorldStart(Entity* entity) {
		entity->OnStart();
	}

	void EngineClass::OnWorldUpdate(Entity* entity) {
		entity->OnUpdate(m_time->DeltaTime());

		Array<SceneComponent*> components;
		entity->GetRootComponent()->GetChildrenComponents(components);
		m_passContext->Process(components);
	}
}
