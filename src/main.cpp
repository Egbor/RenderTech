#include "Engine.h"

#include "Engine/Core/Render/Api/DX11/DX11Context.h"
#include "Engine/Core/System/Platform/Win32/Window.h"

#include "Engine/Core/Render/Api/DX11/DX11Def.h"

#include "Scripts/Spectator.h"
#include "Scripts/VisibleObject.h"
#include "Scripts/LightObject.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	Engine::IWindow* window = new Engine::Win32Window(hInstance, 800, 600);
	Engine::IContext* context = new Engine::DX11Context(window);

	Engine::EngineClass* engine = new Engine::EngineClass(window, context);
	Engine::World* world = engine->GetWorld();

	world->SpawnEntity(Engine::ClassOf<Engine::Spectator>::value, Engine::Vector3(0.0f, 0.0f, -1.0f), Engine::Rotator(0.0f, 0.0f, 0.0f));
	world->SpawnEntity(Engine::ClassOf<Engine::VisibleObject>::value, Engine::Vector3(0.0f, 0.0f, 0.0f), Engine::Rotator(0.0f, 45.0f, 0.0f));
	world->SpawnEntity(Engine::ClassOf<Engine::LightObject>::value, Engine::Vector3(0.0f, 0.0f, -1.0f), Engine::Rotator(0.0f, 0.0f, 0.0f));

	window->Show();
	engine->Run();

	DELETE_OBJECT(engine);
	DELETE_OBJECT(context);
	DELETE_OBJECT(window);

#if defined(DEBUG) || defined(_DEBUG)
	ComPtr<IDXGIDebug1> dxgiDebug;
	if (SUCCEEDED(DXGIGetDebugInterface1(0, IID_IDXGIDebug1, &dxgiDebug))) {
		dxgiDebug->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_FLAGS(DXGI_DEBUG_RLO_DETAIL | DXGI_DEBUG_RLO_IGNORE_INTERNAL));
	}
#endif

	return 0;
}
