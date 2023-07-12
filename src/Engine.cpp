#include "Engine.h"

#include "Engine/Core/Render/Api/DX11/Binder/DX11ViewportBinder.h"

#include "Engine/Object/Global/EngineConfig.h"
#include "Engine/Object/Component/Behavior/MeshComponentBehavior.h"
#include "Engine/Object/Component/Behavior/CameraComponentBehavior.h"
#include "Engine/Object/Component/Behavior/LightComponentBehavior.h"

#include "Engine/Core/System/Serialization/XmlSerializer.h"

#include "Scripts/Spectator.h"
#include "Scripts/VisibleObject.h"
#include "Scripts/LightObject.h"

using namespace Engine;

MeshComponentBehavior* behaviorMesh = new MeshComponentBehavior(ObjectArgument::Dummy());
CameraComponentBehavior* behaviorCamera = new CameraComponentBehavior(ObjectArgument::Dummy());
LightComponentBehavior* behaviorLight = new LightComponentBehavior(ObjectArgument::Dummy());                            

EngineClass::EngineClass(UInt64 winId, UInt32 width, UInt32 height)
    : m_rsScene(nullptr), m_rsGui(nullptr) {
    EngineConfig::GetInstance().Initialize(winId);

    m_time = new Time();
    m_world = new World(ObjectArgument::Dummy());
    m_mode = new WorldMode(ObjectArgument::Dummy());

    m_swapchain = EngineConfig::GetInstance().GetContext()->CreateSwapChain((UInt32)width, (UInt32)height, winId);
    m_rsGui = new GuiRenderSet(EngineConfig::GetInstance().GetGuiContext(), m_swapchain->GetBackBuffer());
    m_rsScene = new SceneRenderSet(EngineConfig::GetInstance().GetContext(), m_rsGui->GetViewportTarget());

    m_world->AddOnEntitySpawnEvent(Delegate<GuiRenderSet, Entity*>::Allocate(m_rsGui, &GuiRenderSet::AddEntityToExplorer));
    m_world->AddOnEntityDestroyEvent(Delegate<GuiRenderSet, Entity*>::Allocate(m_rsGui, &GuiRenderSet::RemoveEntityFromExplorer));
    m_rsGui->AddOnAddObjectEvent(Delegate<EngineClass, const String&>::Allocate(this, &EngineClass::AddObject));
    m_rsGui->AddViewportResizeEvent(Delegate<SceneRenderSet, Int32, Int32>::Allocate(m_rsScene, &SceneRenderSet::SetViewport));
    m_rsGui->SetSwapChain(m_swapchain);

    InitializeWorld();
    InitializeInput(EngineConfig::GetInstance().GetInput());
}

EngineClass::~EngineClass() {
    DELETE_OBJECT(m_rsScene);
    DELETE_OBJECT(m_rsGui);
    DELETE_OBJECT(m_swapchain);
    DELETE_OBJECT(m_time);
    DELETE_OBJECT(m_world);
    DELETE_OBJECT(m_mode);
}

void EngineClass::InvokeStartStage() {
    m_time->Start();
    m_world->Start(m_mode);
}

void EngineClass::InvokeUpdateStage() {
    m_time->Tick();
    m_world->Update(m_time->DeltaTime());
}

void EngineClass::InvokeRenderStage() {
    m_rsScene->Execute();
    m_rsGui->Execute();
}

Context* EngineClass::GetContext() const {
    return EngineConfig::GetInstance().GetContext();
}

GuiContext* EngineClass::GetGuiContext() const {
    return EngineConfig::GetInstance().GetGuiContext();
}

void EngineClass::InitializeWorld() {
    m_mode->SetActor(Spectator::TypeClass());
    m_mode->SetController(EntityController::TypeClass());

    m_world->SpawnEntity(VisibleObject::TypeClass(), Vector3(0.0f, 0.0f, 0.0f), Rotator(0.0f, 0.0f, 0.0f));
    m_world->SpawnEntity(LightObject::TypeClass(), Vector3(0.0f, 0.0f, 0.0f), Rotator(0.0f, 0.0f, 0.0f));
}

void EngineClass::InitializeInput(Input* input) {
    input->SetupAxis("Forword", InputKey::IK_W, 1.0f);
    input->SetupAxis("Forword", InputKey::IK_S, -1.0f);
    input->SetupAxis("Right", InputKey::IK_D, 1.0f);
    input->SetupAxis("Right", InputKey::IK_A, -1.0f);
    input->SetupAxis("RMB", InputKey::IK_MOUSE_BUTTON_RIGHT, 1.0f);
    input->SetupAxis("LookRight", InputKey::IK_MOSUE_X, 1.0f);
    input->SetupAxis("LookUp", InputKey::IK_MOSUE_Y, 1.0f);
}

void EngineClass::AddObject(const String& filename) {
    XmlSerializer serializer(filename, SerializationMode::SM_READ);
    Object* obj = serializer.Read();
    DELETE_OBJECT(obj);
    //ISerializer* root = nullptr;
    //serializer.CreateSubobjectSerializer(&root);

    //Entity* entity = m_world->SpawnEntity(ObjectClassType<Entity>::GetInstance(), Vector3(0.0f, 0.0f, 0.0f), Rotator(0.0f, 0.0f, 0.0f));
    //entity->Deserialize(root);

    //serializer.DispatchSubobjectSerializer(&root);
}
