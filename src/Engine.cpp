#include "Engine.h"

#include "Engine/Core/Render/Api/DX11/Binder/DX11ViewportBinder.h"

#include "Engine/Object/Global/EngineConfig.h"
#include "Engine/Object/Component/Behavior/MeshComponentBehavior.h"
#include "Engine/Object/Component/Behavior/CameraComponentBehavior.h"
#include "Engine/Object/Component/Behavior/LightComponentBehavior.h"

#include "Scripts/Spectator.h"
#include "Scripts/VisibleObject.h"
#include "Scripts/LightObject.h"

#include "Engine/Rendering/RenderPass/MeshRenderPass.h"
#include "Engine/Rendering/RenderPass/LightRenderPass.h"
#include "Engine/Rendering/RenderPass/ShowRenderPass.h"
#include "Engine/Rendering/RenderPass/SkyboxRenderPass.h"

#include "Engine/Core/System/Time/Time.h"

using namespace Engine;

MeshComponentBehavior* behaviorMesh = new MeshComponentBehavior(ObjectArgument::Dummy());
CameraComponentBehavior* behaviorCamera = new CameraComponentBehavior(ObjectArgument::Dummy());
LightComponentBehavior* behaviorLight = new LightComponentBehavior(ObjectArgument::Dummy());

#define START_ENGINE_THREAD(pThread, callback)                                    \
    pThread = new std::thread([&](UInt64 winId_, UInt32 width_, UInt32 height_) { \
        callback(winId_, width_, height_);                                        \
    }, winId, width, height)

#define STOP_ENGINE_THREAD(pThread)  \
    if ((pThread) != nullptr) {      \
        (pThread)->join();           \
        DELETE_OBJECT((pThread));    \
    }                               

EngineClass::EngineClass(UInt64 winId)
    : m_renderThread(nullptr), m_devguiThread(nullptr), m_isRun(false) {
    EngineConfig::GetInstance().Initialize(winId);

    InitializeWorld();
    InitializeInput(EngineConfig::GetInstance().GetInput());
}

EngineClass::~EngineClass() {
    DELETE_OBJECT(m_renderThread);
    DELETE_OBJECT(m_devguiThread);
    DELETE_OBJECT(m_world);
    DELETE_OBJECT(m_mode);
}

void EngineClass::StartRenderLoop(UInt64 winId, UInt32 width, UInt32 height) {
    RenderViewport viewport = { (Int32)width, (Int32)height, 0, 0 };

    Context* context = EngineConfig::GetInstance().GetContext();
    SwapChain* swapchain = context->CreateSwapChain(width, height, winId);

    DX11ViewportBinder* dxViewportBinder = new DX11ViewportBinder(reinterpret_cast<DX11Context*>(context));

    MeshRenderPass* rpMesh = new MeshRenderPass(context, swapchain);
    LightRenderPass* rpLight = new LightRenderPass(context, swapchain);
    ShowRenderPass* rpShow = new ShowRenderPass(context, swapchain);
    SkyboxRenderPass* rpSky = new SkyboxRenderPass(context, swapchain);

    dxViewportBinder->AttachRenderItem((UInt32)0, &viewport);
    dxViewportBinder->Bind();


    m_isRun = true;

    Time time;
    time.Start();

    bool value = true;

    m_world->Start(m_mode);
    while (m_isRun) {
        time.Tick();

        m_world->Update(time.DeltaTime());

        rpMesh->Render(nullptr);
        rpLight->Render(rpMesh);
        rpSky->Render(rpLight);

        swapchain->SwapBuffers();
    }

    DELETE_OBJECT(rpShow);
    DELETE_OBJECT(rpLight);
    DELETE_OBJECT(rpMesh);

    context->DebugReport();
}

void EngineClass::StartDevGuiLoop(UInt64 winId, UInt32 width, UInt32 height) {
    GuiContext* devguiContext = EngineConfig::GetInstance().GetGuiContext();

    bool value = true;

    while (m_isRun) {
    }
}

void EngineClass::Run(UInt64 winId, UInt32 width, UInt32 height) {
    Stop();
    START_ENGINE_THREAD(m_renderThread, StartRenderLoop);
    START_ENGINE_THREAD(m_devguiThread, StartDevGuiLoop);
}

void EngineClass::Stop() {
    m_isRun = false;
    STOP_ENGINE_THREAD(m_renderThread);
    STOP_ENGINE_THREAD(m_devguiThread);
}

void EngineClass::InitializeWorld() {
    m_world = new World(ObjectArgument::Dummy());
    m_mode = new WorldMode(ObjectArgument::Dummy());

    m_mode->SetActor(Spectator::TypeMetaClass());
    m_mode->SetController(EntityController::TypeMetaClass());

    m_world->SpawnEntity(VisibleObject::TypeMetaClass(), Vector3(0.0f, 0.0f, 0.0f), Rotator(0.0f, 0.0f, 0.0f));
    m_world->SpawnEntity(LightObject::TypeMetaClass(), Vector3(0.0f, 0.0f, 0.0f), Rotator(0.0f, 0.0f, 0.0f));
}

void EngineClass::InitializeInput(Input* input) {
    input->SetupAxis("Forword", InputKey::IK_W, 1.0f);
    input->SetupAxis("Forword", InputKey::IK_S, -1.0f);
    input->SetupAxis("Right", InputKey::IK_D, 1.0f);
    input->SetupAxis("Right", InputKey::IK_A, -1.0f);
    input->SetupAxis("LookRight", InputKey::IK_MOSUE_X, 1.0f);
    input->SetupAxis("LookUp", InputKey::IK_MOSUE_Y, 1.0f);
}
