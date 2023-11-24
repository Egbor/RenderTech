#include "Engine/Rendering/Engine/RenderPass/BaseRenderPass.h"
#include "Engine/Core/System/Import/ShaderImport.h"
#include "Engine/Object/Class/Mesh.h"

namespace Engine {
	enum class GBuffer_BaseRenderPass {
		GB_BaseColor,
		GB_ViewNormal,
		GB_ORM,
		GB_Depth,
		GB_NUM
	};

	enum class UBuffer_BaseRenderPass {
		UB_Camera,
		UB_NUM
	};

	BaseRenderPassQueue::BaseRenderPassQueue() 
		: m_uniform(), m_modelBatch() {

	}

	void BaseRenderPassQueue::SetViewProjection(Matrix4x4 viewproj) {
		m_uniform.ViewProjection = viewproj.Transpose();
	}

	Mesh* BaseRenderPassQueue::GetMesh() {
		return m_modelBatch.front().mesh;
	}

	UniformBufferBase& BaseRenderPassQueue::GetUniform() {
		m_uniform.World = m_modelBatch.front().world.Transpose();
		m_uniform.invWorld = m_modelBatch.front().world.Inverse().Transpose();
		return m_uniform;
	}

	bool BaseRenderPassQueue::IsEmpty() const {
		return m_modelBatch.empty();
	}

	void BaseRenderPassQueue::Add(MeshComponent* component) {
		Matrix4x4 world = Matrix4x4::CreateMatrixWorld(component->GetWorldPosition(), component->GetWorldRotation(), component->GetWorldScale());
		m_modelBatch.push({ world, component->GetMesh() });
	}

	void BaseRenderPassQueue::Next() {
		m_modelBatch.pop();
	}

	BaseRenderPass::BaseRenderPass()
		: m_gbuffer(), m_ubuffer(), m_vertexShader(nullptr), m_queue(new BaseRenderPassQueue()) {

	}

	BaseRenderPass::~BaseRenderPass() {
		DELETE_OBJECT(m_queue);
		DELETE_OBJECT(m_vertexShader);
	}

	void BaseRenderPass::Create(IRenderResourceFactory* factory, Int32 width, Int32 height) {
		// GBuffer initialization
		m_gbuffer.Attach(TextureType::TT_DEFAULT, TextureFormat::TF_R8G8B8A8_BMP, width, height);
		m_gbuffer.Attach(TextureType::TT_DEFAULT, TextureFormat::TF_R32G32B32A32_FLOAT, width, height);
		m_gbuffer.Attach(TextureType::TT_DEFAULT, TextureFormat::TF_B8G8R8A8_BMP, width, height);
		m_gbuffer.Attach(TextureType::TT_DEPTH, TextureFormat::TF_R24_BMP_G8_UINT, width, height);

		// Constant buffers initialization
		m_ubuffer.Attach<UniformBufferCameraDesc>(factory);

		// Default shaders initialization
		m_vertexShader = ShaderLoader::Load("assets/shaders/BaseVSShader.vso", ShaderType::ST_VERTEX);
	}

	void BaseRenderPass::Launch(IRenderPipeline* pipeline) {
		IRenderStage* vsStage = pipeline->GetStage(RenderStage::RS_VERTEX);
		IRenderStage* psStage = pipeline->GetStage(RenderStage::RS_PIXEL);

		vsStage->BindBuffers(m_ubuffer.GetBuffers());
		vsStage->BindShader(m_vertexShader);
		pipeline->SetTargets(m_gbuffer.GetTargets());

		m_gbuffer.Clear(pipeline);

		for (; !m_queue->IsEmpty(); m_queue->Next()) {
			Mesh* mesh = m_queue->GetMesh();
			UniformBufferBase& uniform = m_queue->GetUniform();

			pipeline->Update(m_ubuffer.GetBuffer(static_cast<Int32>(UBuffer_BaseRenderPass::UB_Camera)), &uniform, sizeof(UniformBufferBase));

			for (Int32 meshElementId = 0; meshElementId < mesh->GetNumMeshElements(); meshElementId++) {
				MeshElement* element = mesh->GetMeshElement(meshElementId);
				Material* material = mesh->GetMaterial(meshElementId);

				psStage->BindTextures(material->GetNativeTextureResources());
				psStage->BindShader(material->GetNativeShaderResource());
				pipeline->Draw(element->GetVertexBuffer(), element->GetIndexBuffer());
			}
		}
	}

	bool BaseRenderPass::Is(RenderPassType type) const {
		return type == RenderPassType::RP_BASE;
	}

	void BaseRenderPass::AddToQueue(MeshComponent* component) {
		m_queue->Add(component);
	}

	void BaseRenderPass::SetViewProjection(Matrix4x4 viewproj) {
		m_queue->SetViewProjection(viewproj);
	}
}