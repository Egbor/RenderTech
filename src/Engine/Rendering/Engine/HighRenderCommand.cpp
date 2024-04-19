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

	void HighRenderCommandPrePass::Execute(VirtualRenderPipeline* pipeline, Scene* scene) {
		Matrix4x4 view = CreateViewMatrix(scene->GetCamera());
		Matrix4x4 proj = CreateProjectionMatrix(scene->GetCamera(), pipeline->GetRenderSpaceWidth(), pipeline->GetRenderSpaceHeight());

		pipeline->UpdateUBuffer(AS_TEXT(UB_Camera), [&](RawData& data) { UpdatePrePassUBCamera(data, scene->GetCamera(), pipeline->GetRenderSpaceWidth(), pipeline->GetRenderSpaceHeight()); });
		pipeline->UpdateUBuffer(AS_TEXT(UB_Object), [&](RawData& data) { UpdatePrePassUBObject(data, view, proj); });
		pipeline->UpdateUBuffer(AS_TEXT(UB_ObjectHelper), [&](RawData& data) { UpdatePrePassUBOjectHelper(data, view, proj); });
	}

	HighRenderCommandBasePass::HighRenderCommandBasePass(IRenderResourceFactory* factory)
		: m_vertexShader(nullptr) {
		m_vertexShader = LoadShader(factory, "assets/shaders/BaseVSShader.cso", ShaderType::ST_VERTEX);
	}

	HighRenderCommandBasePass::~HighRenderCommandBasePass() {
		DELETE_OBJECT(m_vertexShader);
	}

	void HighRenderCommandBasePass::Execute(VirtualRenderPipeline* pipeline, Scene* scene) {
		pipeline->BindShader(RenderStage::RS_VERTEX, m_vertexShader);
		pipeline->BindUBuffer(BatchSlot::BS_SLOT_1, RenderStage::RS_VERTEX);
		pipeline->BindGBuffer(BatchSlot::BS_SLOT_1);
		pipeline->BindStates(BatchSlot::BS_SLOT_1);

		pipeline->ClearGBuffer(BatchSlot::BS_SLOT_1, true, true);

		scene->DoTraversal(ClassOf<MeshComponent>::value, [&](SceneComponent* component) { DrawSingleMesh(pipeline, component); });
	}

	void HighRenderCommandBasePass::DrawSingleMesh(VirtualRenderPipeline* pipeline, SceneComponent* component) {
		Mesh* mesh = component->As<MeshComponent>()->GetMesh();
		
		pipeline->UpdateUBuffer(AS_TEXT(UB_Object), [&](RawData& data) { UpdateBasePassUBObject(data, component); });
		pipeline->UpdateUBuffer(AS_TEXT(UB_ObjectHelper), [&](RawData& data) { UpdateBasePassUBObjectHelper(data, component); });

		for (Int32 i = 0; i < mesh->GetNumMeshElements(); i++) {
			MeshElement* submesh = mesh->GetMeshElement(i);
			Material* material = mesh->GetMaterial(i);

			pipeline->BindTexture(RenderStage::RS_PIXEL, material->GetNativeTextureResources());
			pipeline->BindShader(RenderStage::RS_PIXEL, material->GetNativeShaderResource());

			pipeline->DrawIndexedPremitive(submesh->GetVertexBuffer(), submesh->GetIndexBuffer());
		}
	}

	HighRenderCommandLightPass::HighRenderCommandLightPass(IRenderResourceFactory* factory) 
		: m_vertexShader(nullptr), m_pixelShader(nullptr) {
		m_vertexShader = LoadShader(factory, "assets/shaders/LightVSShader.cso", ShaderType::ST_VERTEX);
		m_pixelShader = LoadShader(factory, "assets/shaders/LightPSShader.cso", ShaderType::ST_PIXEL);
	}

	HighRenderCommandLightPass::~HighRenderCommandLightPass() {
		DELETE_OBJECT(m_vertexShader);
		DELETE_OBJECT(m_pixelShader);
	}

	void HighRenderCommandLightPass::Execute(VirtualRenderPipeline* pipeline, Scene* scene) {
		pipeline->ClearGBuffer(BatchSlot::BS_SLOT_3, false, false);
		scene->DoTraversal(ClassOf<LightComponent>::value, [&](SceneComponent* component) { DrawSingleLight(pipeline, component); });
	}

	void HighRenderCommandLightPass::DrawSingleLight(VirtualRenderPipeline* pipeline, SceneComponent* component) {
		LightComponent* light = component->As<LightComponent>();
		MeshElement* lightVolume = LoadLightVolume(light->GetLightType())->GetMeshElement(0);

		pipeline->UpdateUBuffer(AS_TEXT(UB_Object), [&](RawData& data) { UpdateBasePassUBObject(data, light); });
		pipeline->UpdateUBuffer(AS_TEXT(UB_ObjectHelper), [&](RawData& data) { UpdateBasePassUBObjectHelper(data, light); });
		pipeline->UpdateUBuffer(AS_TEXT(UB_Light), [&](RawData& data) { UpdateLightPassUBLight(data, light, false); });

		// Front light calculation
		pipeline->BindShader(RenderStage::RS_VERTEX, m_vertexShader);
		pipeline->BindShader(RenderStage::RS_PIXEL, nullptr);
		pipeline->BindGBuffer(BatchSlot::BS_SLOT_2);
		pipeline->BindUBuffer(BatchSlot::BS_SLOT_2, RenderStage::RS_VERTEX);
		pipeline->BindStates(BatchSlot::BS_SLOT_2);

		pipeline->ClearGBuffer(BatchSlot::BS_SLOT_2, false, true, 1);
		pipeline->DrawIndexedPremitive(lightVolume->GetVertexBuffer(), lightVolume->GetIndexBuffer());

		// Back light calculation
		pipeline->BindShader(RenderStage::RS_PIXEL, m_pixelShader);
		pipeline->BindGBuffer(BatchSlot::BS_SLOT_1, RenderStage::RS_PIXEL);
		pipeline->BindGBuffer(BatchSlot::BS_SLOT_3);
		pipeline->BindUBuffer(BatchSlot::BS_SLOT_3, RenderStage::RS_PIXEL);
		pipeline->BindStates(BatchSlot::BS_SLOT_3);

		pipeline->DrawIndexedPremitive(lightVolume->GetVertexBuffer(), lightVolume->GetIndexBuffer());
	}

	HighRenderCommandBakeHDRIToIBL::HighRenderCommandBakeHDRIToIBL(IRenderResourceFactory* factory) 
		: m_vertexShader(nullptr), m_pixelShader(nullptr), m_mat4x4ViewProjection(6) {
		Matrix4x4 proj = Matrix4x4::CreateMatrixOrthographic(-1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 1.0f);
		m_mat4x4ViewProjection[0] = (Matrix4x4::CreateMatrixLookAt(Vector3::zero, -Vector3::right, Vector3::up) * proj).Transpose();
		m_mat4x4ViewProjection[1] = (Matrix4x4::CreateMatrixLookAt(Vector3::zero, Vector3::right, Vector3::up) * proj).Transpose();
		m_mat4x4ViewProjection[2] = (Matrix4x4::CreateMatrixLookAt(Vector3::zero, -Vector3::up, Vector3::forword) * proj).Transpose();
		m_mat4x4ViewProjection[3] = (Matrix4x4::CreateMatrixLookAt(Vector3::zero, Vector3::up, -Vector3::forword) * proj).Transpose();
		m_mat4x4ViewProjection[4] = (Matrix4x4::CreateMatrixLookAt(Vector3::zero, Vector3::forword, Vector3::up) * proj).Transpose();
		m_mat4x4ViewProjection[5] = (Matrix4x4::CreateMatrixLookAt(Vector3::zero, -Vector3::forword, Vector3::up) * proj).Transpose();

		m_vertexShader = LoadShader(factory, "assets/shaders/HDRIToCubeMapVSShader.cso", ShaderType::ST_VERTEX);
		m_pixelShader = LoadShader(factory, "assets/shaders/HDRIToCubeMapPSShader.cso", ShaderType::ST_PIXEL);
	}

	HighRenderCommandBakeHDRIToIBL::~HighRenderCommandBakeHDRIToIBL() {
		DELETE_OBJECT(m_vertexShader);
		DELETE_OBJECT(m_pixelShader);
	}

	void HighRenderCommandBakeHDRIToIBL::Execute(VirtualRenderPipeline* pipeline, Scene* scene) {
		for (Int32 i = 0; i < m_mat4x4ViewProjection.size(); i++) {
			pipeline->UpdateUBuffer(AS_TEXT(UB_Object), [&](RawData& data) { UB_Object* buffer = data.As<UB_Object>(); buffer->ViewProjection = m_mat4x4ViewProjection[i]; });
			pipeline->BindGBuffer(BatchSlot::BS_SLOT_1);
			pipeline->BindShader(RenderStage::RS_VERTEX, m_vertexShader);
			pipeline->BindShader(RenderStage::RS_PIXEL, m_pixelShader);
		}
	}
}