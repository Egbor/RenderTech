#include "Engine.h"

#include "Engine/Core/Render/Api/DX11/DX11Context.h"
#include "Engine/Core/System/Platform/Win32/Window.h"

#include "Engine/Core/Render/Api/DX11/DX11Def.h"

#include "Scripts/Spectator.h"
#include "Scripts/VisibleObject.h"
#include "Scripts/LightObject.h"

//namespace Engine {
//	class TestEngine {
//	public:
//		TestEngine() 
//			: m_runtime(new Runtime()) {
//			m_runtime->DelegateProcess(Delegate<TestEngine, void(Float)>::AllocateDelegate(this, &TestEngine::Test1));
//			m_runtime->DelegateProcess(Delegate<TestEngine, void(Float)>::AllocateDelegate(this, &TestEngine::Test2));
//			m_runtime->DelegateProcess(Delegate<TestEngine, void(Float)>::AllocateDelegate(this, &TestEngine::Test3));
//			m_runtime->DelegateProcess(Delegate<TestEngine, void(Float)>::AllocateDelegate(this, &TestEngine::Test4));
//		}
//
//		~TestEngine() {
//			DELETE_OBJECT(m_runtime);
//		}
//
//		void Start() {
//			m_runtime->Start();
//		}
//
//		void Stop() {
//			m_runtime->Terminate();
//		}
//
//	private:
//		void Test1(Float deltaTime) {
//			std::stringstream ss;
//			ss << "Test1: " << deltaTime << "(" << std::this_thread::get_id() << ")" << std::endl;
//			OutputDebugStringA(ss.str().c_str());
//
//			Sleep(10);
//		}
//
//		void Test2(Float deltaTime) {
//			std::stringstream ss;
//			ss << "Test2: " << deltaTime << "(" << std::this_thread::get_id() << ")" << std::endl;
//			OutputDebugStringA(ss.str().c_str());
//		}
//
//		void Test3(Float deltaTime) {
//			std::stringstream ss;
//			ss << "Test3: " << deltaTime << "(" << std::this_thread::get_id() << ")" << std::endl;
//			OutputDebugStringA(ss.str().c_str());
//
//			Sleep(100);
//		}
//
//		void Test4(Float deltaTime) {
//			std::stringstream ss;
//			ss << "Test4: " << deltaTime << "(" << std::this_thread::get_id() << ")" << std::endl;
//			OutputDebugStringA(ss.str().c_str());
//
//			Sleep(33);
//		}
//
//
//		Runtime* m_runtime;
//	};
//}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	Engine::IWindow* window = new Engine::Win32Window(hInstance, 1280, 720);
	Engine::IContext* context = new Engine::DX11Context(window);
	Engine::EngineRuntime* runtime = new Engine::EngineRuntime(window, context);

	runtime->Init();
	runtime->Start();
	runtime->Terminate();

	DELETE_OBJECT(runtime);
	DELETE_OBJECT(context);
	DELETE_OBJECT(window);
//
//	Engine::EngineClass* engine = new Engine::EngineClass(window, context);
//	//Engine::World* world = engine->GetWorld();
//
//	//world->SpawnEntity(Engine::ClassOf<Engine::Spectator>::value, Engine::Vector3(0.0f, 0.0f, -5.0f), Engine::Rotator(0.0f, 0.0f, 0.0f));
//	//world->SpawnEntity(Engine::ClassOf<Engine::VisibleObject>::value, Engine::Vector3(0.0f, 0.0f, 0.0f), Engine::Rotator(0.0f, 45.0f, 0.0f));
//	//world->SpawnEntity(Engine::ClassOf<Engine::LightObject>::value, Engine::Vector3(0.0f, 0.0f, -8.0f), Engine::Rotator(0.0f, 0.0f, 0.0f));
//
//	window->Show();
//	engine->Run();
//
//	DELETE_OBJECT(engine);
//	DELETE_OBJECT(context);
//	DELETE_OBJECT(window);
//
//#if defined(DEBUG) || defined(_DEBUG)
//	ComPtr<IDXGIDebug1> dxgiDebug;
//	if (SUCCEEDED(DXGIGetDebugInterface1(0, IID_IDXGIDebug1, &dxgiDebug))) {
//		dxgiDebug->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_FLAGS(DXGI_DEBUG_RLO_DETAIL | DXGI_DEBUG_RLO_IGNORE_INTERNAL));
//	}
//#endif

	return 0;
}
