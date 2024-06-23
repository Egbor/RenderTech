#include "Engine.h"

//#include "Engine/Rendering/Engine/RenderPass/BaseRenderPass.h"
//#include "Engine/Rendering/Engine/RenderPass/LightRenderPass.h"

#include "Engine/Core/System/Platform/Common/Input.h"

namespace Engine {
	EngineClass::EngineClass(IWindow* window, IContext* context) {
		Core::GetInstance()->Initialize(window, context);

		m_time = new Time();
		m_world = ClassType<World>::CreateObject(ObjectArgument::Dummy());

		m_context = new HRC_Base(context);
		m_context->DrawInit(context->QueryResourceFactory());

		m_threadpool = new EngineThreadPool(Delegate<EngineClass>::Allocate(this, &EngineClass::SyncEntry), 2);
		m_threadpool->Append(Delegate<EngineClass>::Allocate(this, &EngineClass::GameThreadEntry));
		m_threadpool->Append(Delegate<EngineClass>::Allocate(this, &EngineClass::RenderThreadEntry));
	}

	EngineClass::~EngineClass() {
		DELETE_OBJECT(m_threadpool);
		DELETE_OBJECT(m_context);
		DELETE_OBJECT(m_world);
		DELETE_OBJECT(m_time);
	}

	void EngineClass::Run() {
		m_time->Start();
		m_world->Start();
		m_threadpool->Start();

		IWindow* window = Core::GetInstance()->GetWindow();
		while (!window->HasQuit()) {}

		m_threadpool->Reset();
	}

	World* EngineClass::GetWorld() const {
		return m_world;
	}

	void EngineClass::SyncEntry() {
		m_time->Tick();
	}

	void EngineClass::GameThreadEntry() {
		Input* input = Core::GetInstance()->GetInput();

		m_world->Update(m_time->DeltaTime());
		input->Refresh();
	}

	void EngineClass::RenderThreadEntry() {
		m_context->DrawScene(m_world->GetScene());
	}
}
