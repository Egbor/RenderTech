#include "Engine/Rendering/Engine/HighRenderCommand.h"
#include "Engine/Core/System/Resource/Resource.h"
#include "Engine/Core/System/Exception/EngineException.h"

#include "Engine/Object/Component/MeshComponent.h"
#include "Engine/Object/Component/LightComponent.h"

namespace Engine {
	Matrix4x4 CreateViewMatrix(CameraComponent* component) {
		Vector3 eyePosition = component->GetWorldPosition();
		Vector3 focusPosition = component->GetForward() + eyePosition;
		Vector3 upDirection = component->GetUp();

		return Matrix4x4::CreateMatrixLookAt(eyePosition, focusPosition, upDirection);
	}

	Matrix4x4 CreateProjectionMatrix(CameraComponent* component, Float resolutionWidth, Float resolutionHeight) {
		Float aspectRatio = resolutionWidth / resolutionHeight;

		return Matrix4x4::CreateMatrixPerspective(component->GetFOV(), aspectRatio, component->GetNearZ(), component->GetFarZ());
	}

	IShaderResourceData* LoadShader(IRenderResourceFactory* factory, const String& filename, ShaderType type) {
		const Array<Int8> code = Resource::Load<const Array<Int8>>(filename);
		return factory->CreateShader(type, code.size(), code.data());
	}

	Mesh* LoadLightVolume(LightType type) {
		switch (type) {
		case LightType::LT_POINT:
			static Mesh* pointLight = Resource::Load<Mesh*>("assets/models/Sphere.fbx");
			return pointLight;
		case LightType::LT_SPOT:
			static Mesh* spotLight = Resource::Load<Mesh*>("assets/models/Sphere.fbx");
			return spotLight;
		case LightType::LT_DIRECTIONAL:
			static Mesh* directionalLight = Resource::Load<Mesh*>("assets/models/Sphere.fbx");
			return directionalLight;
		default:
			break;
		}
		throw EngineException("[Global] LoadLightVolume() failed. The selected light is not supported");
	}

	Mesh* LoadCubeVolume() {
		static Mesh* screen = Resource::Load<Mesh*>("assets/models/Cube.fbx");
		return screen;
	}

	Mesh* LoadPlaneVolume() {
		static Mesh* screen = Resource::Load<Mesh*>("assets/models/Plane.fbx");
		return screen;
	}

	void UpdatePrePassUBCamera(RawData& data, CameraComponent* component, Float viewportWidth, Float viewportHeight) {
		UB_Camera* buffer = data.As<UB_Camera>();
		buffer->EyePosition = Vector4(component->GetWorldPosition().x, component->GetWorldPosition().y, component->GetWorldPosition().z, 1.0f);
		buffer->Resolution = Vector2(viewportWidth, viewportHeight);
	}

	void UpdatePrePassUBObject(RawData& data, Matrix4x4 view, Matrix4x4 proj) {
		UB_Object* buffer = data.As<UB_Object>();
		buffer->ViewProjection = (view * proj).Transpose();
	}

	void UpdatePrePassUBOjectHelper(RawData& data, Matrix4x4 view, Matrix4x4 proj) {
		UB_ObjectHelper* buffer = data.As<UB_ObjectHelper>();
		buffer->invView = view.Inverse().Transpose();
		buffer->invProjection = proj.Inverse().Transpose();
	}

	void UpdateBasePassUBObject(RawData& data, SceneComponent* component) {
		UB_Object* buffer = data.As<UB_Object>();
		buffer->World = Matrix4x4::CreateMatrixWorld(component->GetWorldPosition(), component->GetWorldRotation(), component->GetWorldScale()).Transpose();
	}

	void UpdateBasePassUBObjectHelper(RawData& data, SceneComponent* component) {
		UB_ObjectHelper* buffer = data.As<UB_ObjectHelper>();
		buffer->invWorld = Matrix4x4::CreateMatrixWorld(component->GetWorldPosition(), component->GetWorldRotation(), component->GetWorldScale()).Inverse().Transpose();
	}

	void UpdateLightPassUBLight(RawData& data, LightComponent* component, bool isDirectional) {
		UB_Light* buffer = data.As<UB_Light>();
		buffer->Brightness = component->GetBrightness();
		buffer->Color = component->GetColor();
		if (isDirectional) {
			buffer->Value = Vector4(component->GetForward().x, component->GetForward().y, component->GetForward().z, 0.0f);
		} else {
			buffer->Value = Vector4(component->GetWorldPosition().x, component->GetWorldPosition().y, component->GetWorldPosition().y, 1.0f);
		}
	}

	void HighRenderCommandPrePass::Execute(VirtualRenderPipeline* pipeline, const RenderResourcesStorage& storage, Scene* scene) {
		Matrix4x4 view = CreateViewMatrix(scene->GetCamera());
		Matrix4x4 proj = CreateProjectionMatrix(scene->GetCamera(), pipeline->GetRenderSpaceWidth(), pipeline->GetRenderSpaceHeight());

		pipeline->UpdateBuffer(storage.GetResourceFromBatchByTag<IBufferResourceData>(AS_TEXT(UB_Camera)), [&](RawData& data) { UpdatePrePassUBCamera(data, scene->GetCamera(), pipeline->GetRenderSpaceWidth(), pipeline->GetRenderSpaceHeight()); });
		pipeline->UpdateBuffer(storage.GetResourceFromBatchByTag<IBufferResourceData>(AS_TEXT(UB_Object)), [&](RawData& data) { UpdatePrePassUBObject(data, view, proj); });
		pipeline->UpdateBuffer(storage.GetResourceFromBatchByTag<IBufferResourceData>(AS_TEXT(UB_ObjectHelper)), [&](RawData& data) { UpdatePrePassUBOjectHelper(data, view, proj); });
	}

	HighRenderCommandBasePass::HighRenderCommandBasePass(IRenderResourceFactory* factory) {
		m_vertexShader = LoadShader(factory, "assets/shaders/BaseVSShader.cso", ShaderType::ST_VERTEX);
	}

	HighRenderCommandBasePass::~HighRenderCommandBasePass() {
		DELETE_OBJECT(m_vertexShader);
	}

	void HighRenderCommandBasePass::Execute(VirtualRenderPipeline* pipeline, const RenderResourcesStorage& storage, Scene* scene) {
		pipeline->AddResource(RenderStage::RS_VERTEX, m_vertexShader);
		pipeline->AddResources<IStateResourceData>(storage, BatchSlot::BS_SLOT_1, RenderStage::RS_PIXEL);
		pipeline->AddResources<IStateResourceData>(storage, BatchSlot::BS_SLOT_1);
		pipeline->AddResources<ITargetResourceData>(storage, BatchSlot::BS_SLOT_1);

		pipeline->ClearTargets(storage, BatchSlot::BS_SLOT_1);

		scene->DoTraversal(ClassOf<MeshComponent>::value, [&](SceneComponent* component) { DrawSingleMesh(pipeline, storage, component); });
	}

	void HighRenderCommandBasePass::DrawSingleMesh(VirtualRenderPipeline* pipeline, const RenderResourcesStorage& storage, SceneComponent* component) {
		Mesh* mesh = component->As<MeshComponent>()->GetMesh();
		
		pipeline->UpdateBuffer(storage.GetResourceFromBatchByTag<IBufferResourceData>(AS_TEXT(UB_Object)), [&](RawData& data) { UpdateBasePassUBObject(data, component); });
		pipeline->UpdateBuffer(storage.GetResourceFromBatchByTag<IBufferResourceData>(AS_TEXT(UB_ObjectHelper)), [&](RawData& data) { UpdateBasePassUBObjectHelper(data, component); });

		for (Int32 i = 0; i < mesh->GetNumMeshElements(); i++) {
			MeshElement* submesh = mesh->GetMeshElement(i);
			Material* material = mesh->GetMaterial(i);

			pipeline->AddResources(RenderStage::RS_PIXEL, material->GetNativeTextureResources());
			pipeline->AddResource(RenderStage::RS_PIXEL, material->GetNativeShaderResource());

			pipeline->DrawIndexedPremitive(submesh->GetVertexBuffer(), submesh->GetIndexBuffer());
		}
	}

	HighRenderCommandPreLightPass::HighRenderCommandPreLightPass(IRenderResourceFactory* factory) {
		m_vertexShader = LoadShader(factory, "assets/shaders/LightVSShader.cso", ShaderType::ST_VERTEX);
	}

	HighRenderCommandPreLightPass::~HighRenderCommandPreLightPass() {
		DELETE_OBJECT(m_vertexShader);
	}

	void HighRenderCommandPreLightPass::Execute(VirtualRenderPipeline* pipeline, const RenderResourcesStorage& storage, Scene* scene) {
		pipeline->AddResource(RenderStage::RS_VERTEX, m_vertexShader);
		pipeline->AddResourcesFromTargetsToTextures(storage, BatchSlot::BS_SLOT_1, RenderStage::RS_PIXEL);

		Skybox* skybox = scene->GetSkybox();
		if (skybox != nullptr) {
			pipeline->AddResource(RenderStage::RS_PIXEL, skybox->GetIBLNativeResource());
		}
	}

	HighRenderCommandLightPass::HighRenderCommandLightPass(IRenderResourceFactory* factory) {
		m_pixelShaderLightSources = LoadShader(factory, "assets/shaders/LightPSShader.cso", ShaderType::ST_PIXEL);
		m_pixelShaderGlobalIllumination = LoadShader(factory, "assets/shaders/IBLPSShader.cso", ShaderType::ST_PIXEL);
	}

	HighRenderCommandLightPass::~HighRenderCommandLightPass() {
		DELETE_OBJECT(m_pixelShaderLightSources);
		DELETE_OBJECT(m_pixelShaderGlobalIllumination);
	}

	void HighRenderCommandLightPass::Execute(VirtualRenderPipeline* pipeline, const RenderResourcesStorage& storage, Scene* scene) {
		pipeline->SetDepthStencilCleaningFlags(storage.GetResourceFromBatchByTag<ITargetResourceData>(RESOURCE_TAG_DEPTH), false, false);
		pipeline->ClearTargets(storage, BatchSlot::BS_SLOT_3);

		scene->DoTraversal(ClassOf<LightComponent>::value, [&](SceneComponent* component) { DrawSingleLight(pipeline, storage, component); });
	
		Skybox* skybox = scene->GetSkybox();
		MeshElement* plane = LoadPlaneVolume()->GetMeshElement(0);

		if (skybox != nullptr) {
			pipeline->AddResource(RenderStage::RS_PIXEL, m_pixelShaderGlobalIllumination);
			pipeline->AddResources<IStateResourceData>(storage, BatchSlot::BS_SLOT_5);

			Matrix4x4 mat4x4TempViewProjection;
			Matrix4x4 mat4x4TempWorld;

			pipeline->UpdateBuffer(storage.GetResourceFromBatchByTag<IBufferResourceData>(AS_TEXT(UB_Object)), [&](RawData& data) {
				UB_Object* buffer = data.As<UB_Object>();
				mat4x4TempViewProjection = buffer->ViewProjection;
				mat4x4TempWorld = buffer->World;

				buffer->ViewProjection = Matrix4x4::CreateMatrixOrthographic(-1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 1.0f);
				buffer->World = Matrix4x4::Identity();
			});
			pipeline->DrawIndexedPremitive(plane->GetVertexBuffer(), plane->GetIndexBuffer());
			pipeline->UpdateBuffer(storage.GetResourceFromBatchByTag<IBufferResourceData>(AS_TEXT(UB_Object)), [&](RawData& data) {
				UB_Object* buffer = data.As<UB_Object>();
				buffer->ViewProjection = mat4x4TempViewProjection;
				buffer->World = mat4x4TempWorld;
			});
		}
	}

	void HighRenderCommandLightPass::DrawSingleLight(VirtualRenderPipeline* pipeline, const RenderResourcesStorage& storage, SceneComponent* component) {
		LightComponent* light = component->As<LightComponent>();
		MeshElement* lightVolume = LoadLightVolume(light->GetLightType())->GetMeshElement(0);

		pipeline->UpdateBuffer(storage.GetResourceFromBatchByTag<IBufferResourceData>(AS_TEXT(UB_Object)), [&](RawData& data) { UpdateBasePassUBObject(data, light); });
		pipeline->UpdateBuffer(storage.GetResourceFromBatchByTag<IBufferResourceData>(AS_TEXT(UB_ObjectHelper)), [&](RawData& data) { UpdateBasePassUBObjectHelper(data, light); });
		pipeline->UpdateBuffer(storage.GetResourceFromBatchByTag<IBufferResourceData>(AS_TEXT(UB_Light)), [&](RawData& data) { UpdateLightPassUBLight(data, light, false); });

		ITargetResourceData* dstTarget = storage.GetResourceFromBatchByTag<ITargetResourceData>(RESOURCE_TAG_DEPTH);
		storage.GetResourceFromBatchByTag<ITargetResourceData>(RESOURCE_TAG_GBUFFER_DEPTH)->Copy(dstTarget);

		// Front light calculation
		pipeline->AddResource(RenderStage::RS_PIXEL, (IShaderResourceData*)nullptr);
		pipeline->AddResources<IStateResourceData>(storage, BatchSlot::BS_SLOT_2);
		pipeline->AddResources<ITargetResourceData>(storage, BatchSlot::BS_SLOT_2);

		pipeline->SetDepthStencilCleaningFlags(dstTarget, false, true, 1);
		pipeline->ClearTargets(storage, BatchSlot::BS_SLOT_2);
		pipeline->DrawIndexedPremitive(lightVolume->GetVertexBuffer(), lightVolume->GetIndexBuffer());

		// Back light calculation
		pipeline->AddResource(RenderStage::RS_PIXEL, m_pixelShaderLightSources);
		pipeline->AddResources<IStateResourceData>(storage, BatchSlot::BS_SLOT_3);
		pipeline->AddResources<ITargetResourceData>(storage, BatchSlot::BS_SLOT_3);

		pipeline->DrawIndexedPremitive(lightVolume->GetVertexBuffer(), lightVolume->GetIndexBuffer());
	}

	HighRenderCommandSkybox::HighRenderCommandSkybox(IRenderResourceFactory* factory) {
		m_vertexShader = LoadShader(factory, "assets/shaders/SkyboxVSShader.cso", ShaderType::ST_VERTEX);
		m_pixelShader = LoadShader(factory, "assets/shaders/SkyboxPSShader.cso", ShaderType::ST_PIXEL);
	}

	HighRenderCommandSkybox::~HighRenderCommandSkybox() {
		DELETE_OBJECT(m_vertexShader);
		DELETE_OBJECT(m_pixelShader);
	}

	void HighRenderCommandSkybox::Execute(VirtualRenderPipeline* pipeline, const RenderResourcesStorage& storage, Scene* scene) {
		Skybox* skybox = scene->GetSkybox();
		if (skybox == nullptr) {
			return;
		}
		
		MeshElement* cube = LoadCubeVolume()->GetMeshElement(0);

		pipeline->AddResource(RenderStage::RS_VERTEX, m_vertexShader);
		pipeline->AddResource(RenderStage::RS_PIXEL, m_pixelShader);
		pipeline->AddResource(RenderStage::RS_PIXEL, skybox->GetNativeResource());

		pipeline->AddResources<IStateResourceData>(storage, BatchSlot::BS_SLOT_4);
		pipeline->AddResources<ITargetResourceData>(storage, BatchSlot::BS_SLOT_3);

		pipeline->DrawIndexedPremitive(cube->GetVertexBuffer(), cube->GetIndexBuffer());
	}

	HighRenderCommandPostProcessing::HighRenderCommandPostProcessing(IRenderResourceFactory* factory) {
		m_vertexShader = LoadShader(factory, "assets/shaders/LightVSShader.cso", ShaderType::ST_VERTEX);
		m_pixelShaderGammaCorrection = LoadShader(factory, "assets/shaders/PostProcessingPSShader.cso", ShaderType::ST_PIXEL);
	}

	HighRenderCommandPostProcessing::~HighRenderCommandPostProcessing() {
		DELETE_OBJECT(m_vertexShader);
		DELETE_OBJECT(m_pixelShaderGammaCorrection);
	}

	void HighRenderCommandPostProcessing::Execute(VirtualRenderPipeline* pipeline, const RenderResourcesStorage& storage, Scene* scene) {
		MeshElement* plane = LoadPlaneVolume()->GetMeshElement(0);
		
		pipeline->AddResource(RenderStage::RS_VERTEX, m_vertexShader);
		pipeline->AddResource(RenderStage::RS_PIXEL, m_pixelShaderGammaCorrection);
		pipeline->AddResourcesFromTargetsToTextures(storage, BatchSlot::BS_SLOT_3, RenderStage::RS_PIXEL);
		
		pipeline->AddResources<IStateResourceData>(storage, BatchSlot::BS_SLOT_6);
		pipeline->AddResources<ITargetResourceData>(storage, BatchSlot::BS_SLOT_4);

		pipeline->UpdateBuffer(storage.GetResourceFromBatchByTag<IBufferResourceData>(AS_TEXT(UB_Object)), [&](RawData& data) {
			UB_Object* buffer = data.As<UB_Object>();
			buffer->ViewProjection = Matrix4x4::CreateMatrixOrthographic(-1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 1.0f);
			buffer->World = Matrix4x4::Identity();
		});
		pipeline->DrawIndexedPremitive(plane->GetVertexBuffer(), plane->GetIndexBuffer());
	}

	HighRenderCommandBakeHDRIToEnvironmentCubemap::HighRenderCommandBakeHDRIToEnvironmentCubemap(IRenderResourceFactory* factory, ITextureResourceData* equirectangularTexture)
		: m_equirectangularTexture(equirectangularTexture), m_mat4x4ViewProjection(6) {
		Matrix4x4 proj = Matrix4x4::CreateMatrixOrthographic(-1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 1.0f);

		m_mat4x4ViewProjection[0] = Matrix4x4::CreateMatrixLookAt(Vector3::zero, -Vector3::right, Vector3::up) * proj;
		m_mat4x4ViewProjection[1] = Matrix4x4::CreateMatrixLookAt(Vector3::zero, Vector3::right, Vector3::up) * proj;
		m_mat4x4ViewProjection[2] = Matrix4x4::CreateMatrixLookAt(Vector3::zero, -Vector3::up, Vector3::forword) * proj;
		m_mat4x4ViewProjection[3] = Matrix4x4::CreateMatrixLookAt(Vector3::zero, Vector3::up, -Vector3::forword) * proj;
		m_mat4x4ViewProjection[4] = Matrix4x4::CreateMatrixLookAt(Vector3::zero, Vector3::forword, Vector3::up) * proj;
		m_mat4x4ViewProjection[5] = Matrix4x4::CreateMatrixLookAt(Vector3::zero, -Vector3::forword, Vector3::up) * proj;

		m_vertexShader = LoadShader(factory, "assets/shaders/HDRIToEnvironmentVSShader.cso", ShaderType::ST_VERTEX);
		m_pixelShaderEnvironment = LoadShader(factory, "assets/shaders/HDRIToEnvironmentPSShader.cso", ShaderType::ST_PIXEL);
		m_pixelShaderIrradiance = LoadShader(factory, "assets/shaders/HDRIToIrradiancePSShader.cso", ShaderType::ST_PIXEL);
	}

	HighRenderCommandBakeHDRIToEnvironmentCubemap::~HighRenderCommandBakeHDRIToEnvironmentCubemap() {
		DELETE_OBJECT(m_vertexShader);
		DELETE_OBJECT(m_pixelShaderEnvironment);
		DELETE_OBJECT(m_pixelShaderIrradiance);
	}

	void HighRenderCommandBakeHDRIToEnvironmentCubemap::Execute(VirtualRenderPipeline* pipeline, const RenderResourcesStorage& storage, Scene* scene) {
		MeshElement* cube = LoadCubeVolume()->GetMeshElement(0);

		pipeline->AddResource(RenderStage::RS_VERTEX, m_vertexShader);
		pipeline->AddResources<IStateResourceData>(storage, BatchSlot::BS_SLOT_1, RenderStage::RS_PIXEL);
		pipeline->AddResources<IStateResourceData>(storage, BatchSlot::BS_SLOT_1);

		BakeEnvironmentCubemap(pipeline, storage, cube->GetVertexBuffer(), cube->GetIndexBuffer());
		BakeIrradianceCubemap(pipeline, storage,  cube->GetVertexBuffer(), cube->GetIndexBuffer());
	}

	void HighRenderCommandBakeHDRIToEnvironmentCubemap::BakeEnvironmentCubemap(VirtualRenderPipeline* pipeline, const RenderResourcesStorage& storage, IBufferResourceData* cubeVertexBuffer, IBufferResourceData* cubeIndexBuffer) {
		pipeline->SetViewport(1080, 1080);

		pipeline->AddResource(RenderStage::RS_PIXEL, m_pixelShaderEnvironment);
		pipeline->AddResource(RenderStage::RS_PIXEL, m_equirectangularTexture);
		
		for (Int32 i = 0; i < m_mat4x4ViewProjection.size(); i++) {
			pipeline->AddResources<ITargetResourceData>(storage, BatchSlot::BS_SLOT_1);
			pipeline->UpdateBuffer(storage.GetResourceFromBatchByTag<IBufferResourceData>(AS_TEXT(UB_Object)), [&](RawData& data) { UB_Object* buffer = data.As<UB_Object>(); buffer->ViewProjection = m_mat4x4ViewProjection[i]; });
			pipeline->DrawIndexedPremitive(cubeVertexBuffer, cubeIndexBuffer);
		}
	}

	void HighRenderCommandBakeHDRIToEnvironmentCubemap::BakeIrradianceCubemap(VirtualRenderPipeline* pipeline, const RenderResourcesStorage& storage, IBufferResourceData* cubeVertexBuffer, IBufferResourceData* cubeIndexBuffer) {
		pipeline->SetViewport(32, 32);
		
		pipeline->AddResource(RenderStage::RS_PIXEL, m_pixelShaderIrradiance);
		pipeline->AddResourcesFromTargetsToTextures(storage, BatchSlot::BS_SLOT_1, RenderStage::RS_PIXEL);

		for (Int32 i = 0; i < m_mat4x4ViewProjection.size(); i++) {
			pipeline->AddResources<ITargetResourceData>(storage, BatchSlot::BS_SLOT_2);
			pipeline->UpdateBuffer(storage.GetResourceFromBatchByTag<IBufferResourceData>(AS_TEXT(UB_Object)), [&](RawData& data) { UB_Object* buffer = data.As<UB_Object>(); buffer->ViewProjection = m_mat4x4ViewProjection[i]; });
			pipeline->DrawIndexedPremitive(cubeVertexBuffer, cubeIndexBuffer);
		}
	}
}