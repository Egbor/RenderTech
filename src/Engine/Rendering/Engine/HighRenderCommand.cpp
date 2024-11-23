#include "Engine/Rendering/Engine/HighRenderCommand.h"
#include "Engine/Core/System/Exception/EngineException.h"

namespace Engine {
	//IShaderResourceData* LoadShader(IRenderResourceFactory* factory, const String& filename, ShaderType type) {
	//	const Array<Int8> code = Resource::Load<const Array<Int8>>(filename);
	//	return factory->CreateShader(type, code.size(), code.data());
	//}

	//Mesh* LoadLightVolume(LightType type) {
	//	switch (type) {
	//	case LightType::LT_POINT:
	//		static Mesh* pointLight = Resource::Load<Mesh*>("assets/models/Sphere.fbx");
	//		return pointLight;
	//	case LightType::LT_SPOT:
	//		static Mesh* spotLight = Resource::Load<Mesh*>("assets/models/Sphere.fbx");
	//		return spotLight;
	//	case LightType::LT_DIRECTIONAL:
	//		static Mesh* directionalLight = Resource::Load<Mesh*>("assets/models/Sphere.fbx");
	//		return directionalLight;
	//	default:
	//		break;
	//	}
	//	throw EngineException("[Global] LoadLightVolume() failed. The selected light is not supported");
	//}

	//Mesh* LoadCubeVolume() {
	//	static Mesh* screen = Resource::Load<Mesh*>("assets/models/Cube.fbx");
	//	return screen;
	//}

	//Mesh* LoadPlaneVolume() {
	//	static Mesh* screen = Resource::Load<Mesh*>("assets/models/Plane.fbx");
	//	return screen;
	//}

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

	AbstractHighRenderCommand::AbstractHighRenderCommand() {
		m_updater[AS_TEXT(UB_Camera)] = new Delegate<AbstractHighRenderCommand, void(BufferResource*)>(this, &AbstractHighRenderCommand::UpdateUBCamera);
		m_updater[AS_TEXT(UB_Object)] = new Delegate<AbstractHighRenderCommand, void(BufferResource*)>(this, &AbstractHighRenderCommand::UpdateUBObject);
		m_updater[AS_TEXT(UB_ObjectHelper)] = new Delegate<AbstractHighRenderCommand, void(BufferResource*)>(this, &AbstractHighRenderCommand::UpdateUBHelper);
		m_updater[AS_TEXT(UB_Light)] = new Delegate<AbstractHighRenderCommand, void(BufferResource*)>(this, &AbstractHighRenderCommand::UpdateUBLight);
	}

	AbstractHighRenderCommand::~AbstractHighRenderCommand() {
		for (auto it = m_updater.begin(); it != m_updater.end(); it++) {
			DELETE_OBJECT(it->second);
		}
	}

	void AbstractHighRenderCommand::ClearTargets(const HighRenderBatcher& batcher) {
		Array<TargetResource*> targets = batcher.QueryResources<TargetResource>(HRS_Tag::HRS_TARGET);
		for (Size i = 0; i < targets.size(); i++) {
			targets[i]->Clear();
		}
	}

	void AbstractHighRenderCommand::UpdateBuffers(const HighRenderBatcher& batcher) {
		Array<BufferResource*> resources = batcher.QueryResources<BufferResource>(TAG_ANY);
		// Array<NamePlusResourceWrapper<BufferResource>> resources = batcher.QueryNamePlusResources<BufferResource>();

		for (Size i = 0; i < resources.size(); i++) {
			//NamePlusResourceWrapper<BufferResource>& wrapper = resources[i];

			auto it = m_updater.find(resources[i]->GetName());
			if (it != m_updater.end()) {
				it->second->Invoke(resources[i]);
				resources[i]->Update();
			}
		}
	}

	HighRenderCommandPrePass::HighRenderCommandPrePass(const HighRenderStorage& storage)
		: AbstractHighRenderCommand(),  m_batcher() {
		m_batcher.LinkWithStorage(storage, AS_TEXT(UB_Object), TAG_VS_PLUS_PS);
		m_batcher.LinkWithStorage(storage, AS_TEXT(UB_ObjectHelper), TAG_VS_PLUS_PS);
		m_batcher.LinkWithStorage(storage, AS_TEXT(UB_Light), HRS_Tag::HRS_PS_STAGE);
		m_batcher.LinkWithStorage(storage, AS_TEXT(UB_Camera), HRS_Tag::HRS_VS_STAGE);
	}

	void HighRenderCommandPrePass::Execute(HighRenderPipelineAdapter* pipeline, Scene* scene) {
		m_camera = scene->GetCamera();

		pipeline->GetDirectAccessToPipeline()->GetViewport(m_currentResolution);
		pipeline->BindResources(m_batcher.QueryResources<BufferResource>(HRS_Tag::HRS_VS_STAGE), RenderStage::RS_VERTEX);
		pipeline->BindResources(m_batcher.QueryResources<BufferResource>(HRS_Tag::HRS_PS_STAGE), RenderStage::RS_PIXEL);

		UpdateBuffers(m_batcher);
	}

	void HighRenderCommandPrePass::UpdateUBCamera(BufferResource* resource) {
		UB_Camera* buffer = resource->GetBufferData().As<UB_Camera>();
		buffer->EyePosition = Vector4(m_camera->GetWorldPosition(), 1.0f);
		buffer->Resolution = Vector2(m_currentResolution.GetWidth(), m_currentResolution.GetHeight());
	}

	void HighRenderCommandPrePass::UpdateUBObject(BufferResource* resource) {
		Float aspectRatio = static_cast<Float>(m_currentResolution.GetWidth()) / m_currentResolution.GetHeight();
		Matrix4x4 view = Matrix4x4::CreateMatrixLookAt(m_camera->GetWorldPosition(), m_camera->GetForward() + m_camera->GetWorldPosition(), m_camera->GetUp());
		Matrix4x4 proj = Matrix4x4::CreateMatrixPerspective(m_camera->GetFOV(), aspectRatio, m_camera->GetNearZ(), m_camera->GetFarZ());

		UB_Object* buffer = resource->GetBufferData().As<UB_Object>();
		buffer->ViewProjection = (m_view * m_proj).Transpose();
	}

	void HighRenderCommandPrePass::UpdateUBHelper(BufferResource* resource) {
		Float aspectRatio = static_cast<Float>(m_currentResolution.GetWidth()) / m_currentResolution.GetHeight();
		Matrix4x4 view = Matrix4x4::CreateMatrixLookAt(m_camera->GetWorldPosition(), m_camera->GetForward() + m_camera->GetWorldPosition(), m_camera->GetUp());
		Matrix4x4 proj = Matrix4x4::CreateMatrixPerspective(m_camera->GetFOV(), aspectRatio, m_camera->GetNearZ(), m_camera->GetFarZ());

		UB_ObjectHelper* buffer = resource->GetBufferData().As<UB_ObjectHelper>();
		buffer->invView = m_view.Inverse().Transpose();
		buffer->invProjection = m_proj.Inverse().Transpose();
	}

	HighRenderCommandBasePass::HighRenderCommandBasePass(const HighRenderStorage& storage) 
		: AbstractHighRenderCommand(), m_batcher() {
		// Uniform buffers linking
		m_batcher.LinkWithStorage(storage, AS_TEXT(UB_Object), TAG_VS_PLUS_PS);
		m_batcher.LinkWithStorage(storage, AS_TEXT(UB_ObjectHelper), TAG_VS_PLUS_PS);

		// GBuffer linking
		m_batcher.LinkWithStorage(storage, RESOURCE_TAG_GBUFFER_ALBEDO, TAG_GBUFFER);
		m_batcher.LinkWithStorage(storage, RESOURCE_TAG_GBUFFER_NORMAL, TAG_GBUFFER);
		m_batcher.LinkWithStorage(storage, RESOURCE_TAG_GBUFFER_ORM, TAG_GBUFFER);
		m_batcher.LinkWithStorage(storage, RESOURCE_TAG_GBUFFER_DEPTH, TAG_GBUFFER);

		// States linking
		m_batcher.LinkWithStorage(storage, RESOURCE_TAG_STATE_SAMPLER_DEFAULT, TAG_SAMPLER);
		m_batcher.LinkWithStorage(storage, RESOURCE_TAG_STATE_DEPTHSTENCIL_DEFAULT, TAG_STATE);
		m_batcher.LinkWithStorage(storage, RESOURCE_TAG_STATE_RESTERIZER_DEFAULT, TAG_STATE);
		m_batcher.LinkWithStorage(storage, RESOURCE_TAG_STATE_BLEND_DEFAULT, TAG_STATE);
	}

	void HighRenderCommandBasePass::Execute(HighRenderPipelineAdapter* pipeline, Scene* scene) {
		ClearTargets(m_batcher);

		pipeline->BindResources(m_batcher.QueryResources<StateResource>(TAG_SAMPLER), RenderStage::RS_PIXEL);
		pipeline->BindResources(m_batcher.QueryResources<StateResource>(TAG_STATE));
		pipeline->BindResources(m_batcher.QueryResources<TargetResource>(TAG_GBUFFER));

		scene->DoTraversal(ClassOf<MeshComponent>::value, [&](SceneComponent* component) {
			m_component = component;

			UpdateBuffers(m_batcher);
			DrawSingleMesh(pipeline); 
		});
	}

	void HighRenderCommandBasePass::DrawSingleMesh(HighRenderPipelineAdapter* pipeline) {
		StaticMesh* mesh = m_component->As<MeshComponent>()->GetMesh();
		for (Int32 i = 0; i < mesh->GetNumberOfElements(); i++) {
			const MeshUnit& submesh = mesh->GetSubmesh(i);
			const Material* material = mesh->GetMaterial(i);

			pipeline->BindResources(material->GetNativeTextureResources(), RenderStage::RS_PIXEL);
			pipeline->BindResources(material->GetNativeShaderResources());
			pipeline->GetDirectAccessToPipeline()->Draw(submesh.vertexBuffer, submesh.indexBuffer);
		}
	}

	void HighRenderCommandBasePass::UpdateUBObject(BufferResource* resource) {
		UB_Object* buffer = resource->GetBufferData().As<UB_Object>();
		buffer->World = Matrix4x4::CreateMatrixWorld(m_component->GetWorldPosition(), m_component->GetWorldRotation(), m_component->GetWorldScale()).Transpose();
	}

	void HighRenderCommandBasePass::UpdateUBHelper(BufferResource* resource) {
		UB_ObjectHelper* buffer = resource->GetBufferData().As<UB_ObjectHelper>();
		buffer->invWorld = Matrix4x4::CreateMatrixWorld(m_component->GetWorldPosition(), m_component->GetWorldRotation(), m_component->GetWorldScale()).Inverse().Transpose();
	}

	HighRenderCommandLightPass::HighRenderCommandLightPass(const HighRenderStorage& storage) {
		// Uniform buffers linking
		m_batcher.LinkWithStorage(storage, AS_TEXT(UB_Object), TAG_VS_PLUS_PS);
		m_batcher.LinkWithStorage(storage, AS_TEXT(UB_ObjectHelper), TAG_VS_PLUS_PS);
		m_batcher.LinkWithStorage(storage, AS_TEXT(UB_Light), HRS_Tag::HRS_PS_STAGE);

		// Targets linking
		m_batcher.LinkWithStorage(storage, RESOURCE_TAG_FRAME_HDR, HRS_Tag::HRS_TARGET | HRS_Tag::HRS_ADDITION_1);
		m_batcher.LinkWithStorage(storage, RESOURCE_TAG_DEPTH, HRS_Tag::HRS_TARGET | HRS_Tag::HRS_ADDITION_2);

		// Targets linking to transform to shader resources
		m_batcher.LinkWithStorage(storage, RESOURCE_TAG_GBUFFER_ALBEDO, HRS_Tag::HRS_PS_STAGE);
		m_batcher.LinkWithStorage(storage, RESOURCE_TAG_GBUFFER_NORMAL, HRS_Tag::HRS_PS_STAGE);
		m_batcher.LinkWithStorage(storage, RESOURCE_TAG_GBUFFER_ORM, HRS_Tag::HRS_PS_STAGE);
		m_batcher.LinkWithStorage(storage, RESOURCE_TAG_GBUFFER_DEPTH, HRS_Tag::HRS_PS_STAGE);

		// States linking for front calculation
		m_batcher.LinkWithStorage(storage, RESOURCE_TAG_STATE_BLEND_FRONT, TAG_STATE | HRS_Tag::HRS_ADDITION_1);
		m_batcher.LinkWithStorage(storage, RESOURCE_TAG_STATE_RESTERIZER_FRONT, TAG_STATE | HRS_Tag::HRS_ADDITION_1);
		m_batcher.LinkWithStorage(storage, RESOURCE_TAG_STATE_DEPTHSTENCIL_FRONT, TAG_STATE | HRS_Tag::HRS_ADDITION_1);

		// States linking for back calculation
		m_batcher.LinkWithStorage(storage, RESOURCE_TAG_STATE_BLEND_BACK, TAG_STATE | HRS_Tag::HRS_ADDITION_2);
		m_batcher.LinkWithStorage(storage, RESOURCE_TAG_STATE_RESTERIZER_BACK, TAG_STATE | HRS_Tag::HRS_ADDITION_2);
		m_batcher.LinkWithStorage(storage, RESOURCE_TAG_STATE_DEPTHSTENCIL_BACK, TAG_STATE | HRS_Tag::HRS_ADDITION_2);
	}

	void HighRenderCommandLightPass::Execute(HighRenderPipelineAdapter* pipeline, Scene* scene) {
		DepthStencilResource* dstDepth = dynamic_cast<DepthStencilResource*>(m_batcher.QueryResourceByName<TargetResource>(RESOURCE_TAG_DEPTH));
		DepthStencilResource* srcDepth = dynamic_cast<DepthStencilResource*>(m_batcher.QueryResourceByName<TargetResource>(RESOURCE_TAG_GBUFFER_DEPTH));

		srcDepth->Copy(dstDepth);
		dstDepth->DisableDepthClear();
		dstDepth->SetStencilClearValue(1);

		scene->DoTraversal(ClassOf<LightComponent>::value, [&](SceneComponent* component) { 
			m_component = component->As<LightComponent>();

			UpdateBuffers(m_batcher);
			ClearTargets(m_batcher);
			DrawSingleLight(pipeline); 
		});
	}

	void HighRenderCommandLightPass::DrawSingleLight(HighRenderPipelineAdapter* pipeline) {
		StaticMesh* lightArea = m_component->GetLightArea();

		assert(lightArea != nullptr);

		const MeshUnit& lightMesh = lightArea->GetSubmesh(0);
		const Material* lightMaterial = lightArea->GetMaterial(0);

		// Calculate front light area
		pipeline->DisableRenderStage(RenderStage::RS_PIXEL);
		pipeline->BindResources<StateResource>(m_batcher.QueryResources<StateResource>(HRS_Tag::HRS_ADDITION_1));
		pipeline->BindResources<TargetResource>(m_batcher.QueryResources<TargetResource>(HRS_Tag::HRS_ADDITION_2));
		pipeline->BindResources<ShaderResource>(lightMaterial->GetNativeShaderResources());
		pipeline->GetDirectAccessToPipeline()->Draw(lightMesh.vertexBuffer, lightMesh.indexBuffer);

		// Calculate back light area
		pipeline->EnableRenderStage(RenderStage::RS_PIXEL);
		pipeline->BindResources<StateResource>(m_batcher.QueryResources<StateResource>(HRS_Tag::HRS_ADDITION_2));
		pipeline->BindResources<TargetResource>(m_batcher.QueryResources<TargetResource>(HRS_Tag::HRS_TARGET));
		pipeline->BindResources<ShaderResource>(lightMaterial->GetNativeShaderResources());
		pipeline->GetDirectAccessToPipeline()->Draw(lightMesh.vertexBuffer, lightMesh.indexBuffer);
	}

	void HighRenderCommandLightPass::UpdateUBObject(BufferResource* resource) {
		UB_Object* buffer = resource->GetBufferData().As<UB_Object>();
		buffer->World = Matrix4x4::CreateMatrixWorld(m_component->GetWorldPosition(), m_component->GetWorldRotation(), m_component->GetWorldScale()).Transpose();
	}

	void HighRenderCommandLightPass::UpdateUBHelper(BufferResource* resource) {
		UB_ObjectHelper* buffer = resource->GetBufferData().As<UB_ObjectHelper>();
		buffer->invWorld = Matrix4x4::CreateMatrixWorld(m_component->GetWorldPosition(), m_component->GetWorldRotation(), m_component->GetWorldScale()).Inverse().Transpose();
	}

	void HighRenderCommandLightPass::UpdateUBLight(BufferResource* resource) {
		UB_Light* buffer = resource->GetBufferData().As<UB_Light>();
		buffer->Brightness = m_component->GetBrightness();
		buffer->Color = m_component->GetColor();

		if (m_component->GetLightType() == LightType::LT_DIRECTIONAL) {
			buffer->Value = Vector4(m_component->GetForward(), 0.0f);
		} else {
			buffer->Value = Vector4(m_component->GetWorldPosition(), 1.0f);
		}
	}

	//HighRenderCommandSkybox::HighRenderCommandSkybox(const HighRenderStorage& storage) {
	//	//m_vertexShader = LoadShader(factory, "assets/shaders/SkyboxVSShader.cso", ShaderType::ST_VERTEX);
	//	//m_pixelShader = LoadShader(factory, "assets/shaders/SkyboxPSShader.cso", ShaderType::ST_PIXEL);

	//	// Uniform buffers linking
	//	m_batcher.LinkWithStorage(storage, AS_TEXT(UB_Object), TAG_VS_PLUS_PS);

	//	// Targets linking
	//	m_batcher.LinkWithStorage(storage, RESOURCE_TAG_FRAME_HDR, HRS_Tag::HRS_TARGET);
	//	m_batcher.LinkWithStorage(storage, RESOURCE_TAG_DEPTH, HRS_Tag::HRS_TARGET);

	//	// States linking
	//	m_batcher.LinkWithStorage(storage, RESOURCE_TAG_STATE_BLEND_BACK, TAG_STATE | HRS_Tag::HRS_ADDITION_1);
	//	m_batcher.LinkWithStorage(storage, RESOURCE_TAG_STATE_RESTERIZER_FRONT, TAG_STATE | HRS_Tag::HRS_ADDITION_1);
	//	m_batcher.LinkWithStorage(storage, RESOURCE_TAG_STATE_RESTERIZER_BACK, TAG_STATE | HRS_Tag::HRS_ADDITION_2);
	//	m_batcher.LinkWithStorage(storage, RESOURCE_TAG_STATE_DEPTHSTENCIL_SKYBOX, TAG_STATE | HRS_Tag::HRS_ADDITION_1 | HRS_Tag::HRS_ADDITION_2);
	//}

	////HighRenderCommandSkybox::~HighRenderCommandSkybox() {
	////	DELETE_OBJECT(m_vertexShader);
	////	DELETE_OBJECT(m_pixelShader);
	////}

	//void HighRenderCommandSkybox::Execute(HighRenderPipelineAdapter* pipeline, Scene* scene) {
	//	Skybox* skybox = scene->GetSkybox();
	//	if (skybox != nullptr) {
	//		pipeline->BindResources<ITargetResourceData>(m_batcher.QueryResources<ITargetResourceData>(HRS_Tag::HRS_TARGET));
	//		pipeline->BindResources<ITextureResourceData>();

	//		pipeline->BindResources<IStateResourceData>(m_batcher.QueryResources<IStateResourceData>(HRS_Tag::HRS_ADDITION_2));
	//		pipeline->BindResources<IShaderResourceData>();
	//		pipeline->GetDirectAccessToPipeline()->Draw();

	//		UpdateBuffers(m_batcher);

	//		pipeline->BindResources<IStateResourceData>(m_batcher.QueryResources<IStateResourceData>(HRS_Tag::HRS_ADDITION_1));
	//		pipeline->BindResources<IShaderResourceData>();
	//		pipeline->GetDirectAccessToPipeline()->Draw();
	//	}
	//	
	//	//MeshElement* cube = LoadCubeVolume()->GetMeshElement(0);

	//	//pipeline->AddResource(RenderStage::RS_VERTEX, m_vertexShader);
	//	//pipeline->AddResource(RenderStage::RS_PIXEL, m_pixelShader);
	//	//pipeline->AddResource(RenderStage::RS_PIXEL, skybox->GetNativeResource());

	//	//pipeline->AddResources<IStateResourceData>(storage, BatchSlot::BS_SLOT_4);
	//	//pipeline->AddResources<ITargetResourceData>(storage, BatchSlot::BS_SLOT_3);

	//	//pipeline->DrawIndexedPremitive(cube->GetVertexBuffer(), cube->GetIndexBuffer());
	//}

	//HighRenderCommandPostProcessing::HighRenderCommandPostProcessing(IRenderResourceFactory* factory) {
	//	m_vertexShader = LoadShader(factory, "assets/shaders/LightVSShader.cso", ShaderType::ST_VERTEX);
	//	m_pixelShaderGammaCorrection = LoadShader(factory, "assets/shaders/PostProcessingPSShader.cso", ShaderType::ST_PIXEL);
	//}

	//HighRenderCommandPostProcessing::~HighRenderCommandPostProcessing() {
	//	DELETE_OBJECT(m_vertexShader);
	//	DELETE_OBJECT(m_pixelShaderGammaCorrection);
	//}

	//void HighRenderCommandPostProcessing::Execute(VirtualRenderPipeline* pipeline, const RenderResourcesStorage& storage, Scene* scene) {
	//	MeshElement* plane = LoadPlaneVolume()->GetMeshElement(0);
	//	
	//	pipeline->AddResource(RenderStage::RS_VERTEX, m_vertexShader);
	//	pipeline->AddResource(RenderStage::RS_PIXEL, m_pixelShaderGammaCorrection);
	//	pipeline->AddResourcesFromTargetsToTextures(storage, BatchSlot::BS_SLOT_3, RenderStage::RS_PIXEL);
	//	
	//	pipeline->AddResources<IStateResourceData>(storage, BatchSlot::BS_SLOT_6);
	//	pipeline->AddResources<ITargetResourceData>(storage, BatchSlot::BS_SLOT_4);

	//	pipeline->UpdateBuffer(storage.GetResourceFromBatchByTag<IBufferResourceData>(AS_TEXT(UB_Object)), [&](RawData& data) {
	//		UB_Object* buffer = data.As<UB_Object>();
	//		buffer->ViewProjection = Matrix4x4::CreateMatrixOrthographic(-1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 1.0f);
	//		buffer->World = Matrix4x4::Identity();
	//	});
	//	pipeline->DrawIndexedPremitive(plane->GetVertexBuffer(), plane->GetIndexBuffer());
	//}

	HighRenderCommandBakeHDRIToEnvironmentCubemap::HighRenderCommandBakeHDRIToEnvironmentCubemap(const HighRenderStorage& storage)
		: AbstractHighRenderCommand(), m_mat4x4ViewProjection(6), m_faceId(0) {
		Matrix4x4 proj = Matrix4x4::CreateMatrixOrthographic(-1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 1.0f);

		m_mat4x4ViewProjection[0] = Matrix4x4::CreateMatrixLookAt(Vector3::zero, -Vector3::right, Vector3::up) * proj;
		m_mat4x4ViewProjection[1] = Matrix4x4::CreateMatrixLookAt(Vector3::zero, Vector3::right, Vector3::up) * proj;
		m_mat4x4ViewProjection[2] = Matrix4x4::CreateMatrixLookAt(Vector3::zero, -Vector3::up, Vector3::forword) * proj;
		m_mat4x4ViewProjection[3] = Matrix4x4::CreateMatrixLookAt(Vector3::zero, Vector3::up, -Vector3::forword) * proj;
		m_mat4x4ViewProjection[4] = Matrix4x4::CreateMatrixLookAt(Vector3::zero, Vector3::forword, Vector3::up) * proj;
		m_mat4x4ViewProjection[5] = Matrix4x4::CreateMatrixLookAt(Vector3::zero, -Vector3::forword, Vector3::up) * proj;

		m_batcher.LinkWithStorage(storage, RESOURCE_TAG_BAKING_ENV_CUBEMAP, HRS_Tag::HRS_TARGET | HRS_Tag::HRS_ADDITION_1);
		m_batcher.LinkWithStorage(storage, RESOURCE_TAG_BAKING_IRR_CUBEMAP, HRS_Tag::HRS_TARGET | HRS_Tag::HRS_ADDITION_2);
		m_batcher.LinkWithStorage(storage, RESOURCE_TAG_STATE_SAMPLER_DEFAULT, TAG_SAMPLER);
		m_batcher.LinkWithStorage(storage, RESOURCE_TAG_STATE_RESTERIZER_BACK, TAG_STATE);
		m_batcher.LinkWithStorage(storage, AS_TEXT(UB_Object), HRS_Tag::HRS_VS_STAGE);
	}

	//HighRenderCommandBakeHDRIToEnvironmentCubemap::~HighRenderCommandBakeHDRIToEnvironmentCubemap() {
	//	DELETE_OBJECT(m_vertexShader);
	//	DELETE_OBJECT(m_pixelShaderEnvironment);
	//	DELETE_OBJECT(m_pixelShaderIrradiance);
	//}

	void HighRenderCommandBakeHDRIToEnvironmentCubemap::Execute(HighRenderPipelineAdapter* pipeline, Scene* scene) {
		StaticMesh* staticCubeMesh = Core::Load<StaticMesh>("models\\Cube.rtasset");

		pipeline->BindResources(m_batcher.QueryResources<StateResource>(HRS_Tag::HRS_PS_STAGE), RenderStage::RS_PIXEL);
		pipeline->BindResources(m_batcher.QueryResources<StateResource>(HRS_Tag::HRS_STANDALONE));

		BakeEnvironmentCubemap(pipeline, staticCubeMesh->GetSubmesh(0), Core::Load<Material>("materials\\baking\\RawEnvironmentSkybox.rtasset"));
		BakeIrradianceCubemap(pipeline, staticCubeMesh->GetSubmesh(0), Core::Load<Material>("materials\\baking\\RawIrradianceSkybox.rtasset"));
	}

	void HighRenderCommandBakeHDRIToEnvironmentCubemap::BakeEnvironmentCubemap(HighRenderPipelineAdapter* pipeline, const MeshUnit& mesh, const Material* material) {
		pipeline->SetViewportResolution(1080, 1080);
		pipeline->BindResources(material->GetNativeShaderResources());
		pipeline->BindResources(material->GetNativeTextureResources());
		
		for (m_faceId = 0; m_faceId < m_mat4x4ViewProjection.size(); m_faceId++) {
			UpdateBuffers(m_batcher);

			Array<TargetResource*> targets = m_batcher.QueryResources<TargetResource>(HRS_Tag::HRS_ADDITION_1);
			targets[0]->SelectFace(static_cast<TextureFace>(m_faceId));

			pipeline->BindResources(targets);
			pipeline->GetDirectAccessToPipeline()->Draw(mesh.vertexBuffer, mesh.indexBuffer);
		}
	}

	void HighRenderCommandBakeHDRIToEnvironmentCubemap::BakeIrradianceCubemap(HighRenderPipelineAdapter* pipeline, const MeshUnit& mesh, const Material* material) {
		pipeline->SetViewportResolution(32, 32);
		pipeline->BindResources(material->GetNativeShaderResources());
		pipeline->BindResources(m_batcher.QueryResources<TextureResource>(HRS_Tag::HRS_ADDITION_1), RenderStage::RS_PIXEL);

		for (m_faceId = 0; m_faceId < m_mat4x4ViewProjection.size(); m_faceId++) {
			UpdateBuffers(m_batcher);

			Array<TargetResource*> targets = m_batcher.QueryResources<TargetResource>(HRS_Tag::HRS_ADDITION_2);
			targets[0]->SelectFace(static_cast<TextureFace>(m_faceId));

			pipeline->BindResources(targets);
			pipeline->GetDirectAccessToPipeline()->Draw(mesh.vertexBuffer, mesh.indexBuffer);
		}
	}

	void HighRenderCommandBakeHDRIToEnvironmentCubemap::UpdateUBObject(BufferResource* resource) {
		UB_Object* buffer = resource->GetBufferData().As<UB_Object>();
		buffer->ViewProjection = m_mat4x4ViewProjection[m_faceId];
	}
}