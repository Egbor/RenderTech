#include "Engine.h"
#include "Engine/Core/Core.h"

namespace Engine {
	EngineClass::EngineClass(IWindow* window, IContext* context) 
		: m_time(new Time()) {
		Core::GetInstance()->Initialize(window, context);
	}

	EngineClass::~EngineClass() {
		DELETE_OBJECT(m_time);
	}

	void EngineClass::Run(World* world) {
		static EventBase<Entity*>& onStart = Delegate<EngineClass, Entity*>::Allocate(this, &EngineClass::OnWorldStart);
		static EventBase<Entity*>& onUpdate = Delegate<EngineClass, Entity*>::Allocate(this, &EngineClass::OnWorldUpdate);

		IWindow* window = Core::GetInstance()->GetWindow();

		m_time->Start();

		world->ForEachEntity(onStart);
		while (!window->HasQuit()) {
			world->ForEachEntity(onUpdate);

			//m_context->Render();
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
		//m_context->Process(components);
	}
}
