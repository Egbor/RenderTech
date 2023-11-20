#include "Engine.h"

//#include "Engine/Core/Render/Api/DX11/Binder/DX11ViewportBinder.h"
//
//#include "Engine/Object/Global/EngineConfig.h"
//#include "Engine/Object/Component/Behavior/MeshComponentBehavior.h"
//#include "Engine/Object/Component/Behavior/CameraComponentBehavior.h"
//#include "Engine/Object/Component/Behavior/LightComponentBehavior.h"
//
//#include "Engine/Core/System/Serialization/XmlSerializer.h"
//
//#include "Scripts/Spectator.h"
//#include "Scripts/VisibleObject.h"
//#include "Scripts/LightObject.h"

#include "Engine/Core/Render/Api/DX11/DX11Context.h"

#include "Engine/Rendering/Engine/RenderPass/BaseRenderPass.h"
#include "Engine/Rendering/Engine/RenderPass/RenderPassContext.h"

using namespace Engine;

EngineClass::EngineClass(IWindow* window) 
	: m_window(window) {
	m_time = new Time();
	m_context = new RenderPassContext<DX11Context>(window);

	//m_context->Append(new BaseRenderPass());
}

EngineClass::~EngineClass() {
	DELETE_OBJECT(m_context);
	DELETE_OBJECT(m_time);
}

void EngineClass::Run(World* world) {
	static EventBase<Entity*>& onStart = Delegate<EngineClass, Entity*>::Allocate(this, &EngineClass::OnWorldStart);
	static EventBase<Entity*>& onUpdate = Delegate<EngineClass, Entity*>::Allocate(this, &EngineClass::OnWorldUpdate);

	m_time->Start();

	world->ForEachEntity(onStart);
	while (!m_window->HasQuit()) {
		world->ForEachEntity(onUpdate);

		m_context->Render();
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
	m_context->Process(components);
}

//MeshComponentBehavior* behaviorMesh = new MeshComponentBehavior(ObjectArgument::Dummy());
//CameraComponentBehavior* behaviorCamera = new CameraComponentBehavior(ObjectArgument::Dummy());
//LightComponentBehavior* behaviorLight = new LightComponentBehavior(ObjectArgument::Dummy());                            
//
//EngineClass::EngineClass(UInt64 winId, UInt32 width, UInt32 height)
//    : m_rsScene(nullptr), m_rsGui(nullptr) {
//    EngineConfig::GetInstance().Initialize(winId);
//
//    m_time = new Time();
//    m_world = new World(ObjectArgument::Dummy());
//    m_mode = new WorldMode(ObjectArgument::Dummy());
//
//    m_swapchain = EngineConfig::GetInstance().GetContext()->CreateSwapChain((UInt32)width, (UInt32)height, winId);
//    m_rsGui = new GuiRenderSet(EngineConfig::GetInstance().GetGuiContext(), m_swapchain->GetBackBuffer());
//    m_rsScene = new SceneRenderSet(EngineConfig::GetInstance().GetContext(), m_rsGui->GetViewportTarget());
//
//    m_world->AddOnEntitySpawnEvent(Delegate<GuiRenderSet, Entity*>::Allocate(m_rsGui, &GuiRenderSet::AddEntityToExplorer));
//    m_world->AddOnEntityDestroyEvent(Delegate<GuiRenderSet, Entity*>::Allocate(m_rsGui, &GuiRenderSet::RemoveEntityFromExplorer));
//    m_rsGui->AddOnAddObjectEvent(Delegate<EngineClass, const String&>::Allocate(this, &EngineClass::AddObject));
//    m_rsGui->AddViewportResizeEvent(Delegate<SceneRenderSet, Int32, Int32>::Allocate(m_rsScene, &SceneRenderSet::SetViewport));
//    m_rsGui->SetSwapChain(m_swapchain);
//
//    InitializeWorld();
//    InitializeInput(EngineConfig::GetInstance().GetInput());
//}
//
//EngineClass::~EngineClass() {
//    DELETE_OBJECT(m_rsScene);
//    DELETE_OBJECT(m_rsGui);
//    DELETE_OBJECT(m_swapchain);
//    DELETE_OBJECT(m_time);
//    DELETE_OBJECT(m_world);
//    DELETE_OBJECT(m_mode);
//}
//
//void EngineClass::InvokeStartStage() {
//    m_time->Start();
//    m_world->Start(m_mode);
//}
//
//void EngineClass::InvokeUpdateStage() {
//    m_time->Tick();
//    m_world->Update(m_time->DeltaTime());
//}
//
//void EngineClass::InvokeRenderStage() {
//    m_rsScene->Execute();
//    m_rsGui->Execute();
//}
//
//Context* EngineClass::GetContext() const {
//    return EngineConfig::GetInstance().GetContext();
//}
//
//GuiContext* EngineClass::GetGuiContext() const {
//    return EngineConfig::GetInstance().GetGuiContext();
//}
//
//void EngineClass::InitializeWorld() {
//    m_mode->SetActor(ClassOf<Spectator>::value);
//    m_mode->SetController(ClassOf<EntityController>::value);
//
//    m_world->SpawnEntity(ClassOf<VisibleObject>::value, Vector3(0.0f, 0.0f, 0.0f), Rotator(0.0f, 0.0f, 0.0f));
//    m_world->SpawnEntity(ClassOf<LightObject>::value, Vector3(0.0f, 0.0f, 0.0f), Rotator(0.0f, 0.0f, 0.0f));
//}
//
//void EngineClass::InitializeInput(Input* input) {
//    input->SetupAxis("Forword", InputKey::IK_W, 1.0f);
//    input->SetupAxis("Forword", InputKey::IK_S, -1.0f);
//    input->SetupAxis("Right", InputKey::IK_D, 1.0f);
//    input->SetupAxis("Right", InputKey::IK_A, -1.0f);
//    input->SetupAxis("RMB", InputKey::IK_MOUSE_BUTTON_RIGHT, 1.0f);
//    input->SetupAxis("LookRight", InputKey::IK_MOSUE_X, 1.0f);
//    input->SetupAxis("LookUp", InputKey::IK_MOSUE_Y, 1.0f);
//}
//
//void EngineClass::AddObject(const String& filename) {
//    XmlSerializer serializer(filename, SerializationMode::SM_READ);
//    Object* obj = serializer.Read();
//    DELETE_OBJECT(obj);
//}
