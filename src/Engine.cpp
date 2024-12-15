#include "Engine.h"
#include "Engine/Core/System/Platform/Common/Input.h"

#include <sstream>

namespace Engine {
	EngineClass::EngineClass(IWindow* window, IContext* context) {
		Core::GetInstance()->Initialize(window, context);

		m_time = new Time();
		m_world = ClassType<World>::CreateObject(ObjectArgument::Dummy());

		m_context = new HRC_IBLBacker(context, "", 1080, 1080); // new HRC_Base(context);
		//m_context->DrawInit(context->QueryResourceFactory());

		m_threadpool = new EngineThreadPool(Callable<void()>::AllocateDelegate<EngineClass>(this, &EngineClass::SyncEntry), 2);
		m_threadpool->Append(Callable<void()>::AllocateDelegate<EngineClass>(this, &EngineClass::GameThreadEntry));
		m_threadpool->Append(Callable<void()>::AllocateDelegate<EngineClass>(this, &EngineClass::RenderThreadEntry));
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

		//IWindow* window = Core::GetInstance()->GetWindow();
		//while (!window->HasQuit()) {}

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

	EngineRuntime::EngineRuntime(IWindow* window, IContext* context)
		: Runtime(), m_context(nullptr) {
		Core::GetInstance()->Initialize(window, context);

		DelegateProcess(Delegate<EngineRuntime, void(Float)>::AllocateDelegate(this, &EngineRuntime::UpdateLoop));
		DelegateProcess(Delegate<EngineRuntime, void(Float)>::AllocateDelegate(this, &EngineRuntime::RenderLoop));
	}

	EngineRuntime::~EngineRuntime() {
		DELETE_OBJECT(m_context);
	}

	void EngineRuntime::Init() {
		m_context = new HRC_IBLBacker(Core::GetInstance()->GetContext(), "", 1080, 1080);
	}

	void EngineRuntime::Start() {
		Runtime::Start();
		IWindow* window = Core::GetInstance()->GetWindow();

		window->Show();
		while (!window->HasQuit()) {}
	}

	void EngineRuntime::UpdateLoop(Float deltaTime) {
		std::stringstream ss;
		ss << "UpdateLoop: " << deltaTime << "(" << std::this_thread::get_id() << ")" << std::endl;
		OutputDebugStringA(ss.str().c_str());
	}

	void EngineRuntime::RenderLoop(Float deltaTime) {
		m_context->DrawScene(/*scene*/ nullptr);
	}
}
