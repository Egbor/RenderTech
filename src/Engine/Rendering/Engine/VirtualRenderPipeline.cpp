#include "Engine/Rendering/Engine/VirtualRenderPipeline.h"

namespace Engine {
	VirtualRenderPipeline::VirtualRenderPipeline(IContext* apiContext)
		: m_apiContext(apiContext), m_localGBuffer(), m_localStates()
		, m_localStages(static_cast<Int32>(RenderStage::RS_NUM)) {
		ISwapChain* swapchain = apiContext->QuerySwapChain();
		m_localViewport = Viewport(swapchain->GetWidth(), swapchain->GetHeight());
	}

	void VirtualRenderPipeline::DrawIndexedPremitive(IBufferResourceData* vertex, IBufferResourceData* index) {
		AdjustViewport();
		SetApiPipeline();

		m_apiContext->QueryPipeline()->Draw(vertex, index);
	}

	void VirtualRenderPipeline::DrawIndexedWaveframe(IBufferResourceData* vertex, IBufferResourceData* index) {
		AdjustViewport();
		SetApiPipeline();

		m_apiContext->QueryPipeline()->DrawWaveframe(vertex, index);
	}

	void VirtualRenderPipeline::SwapBuffers() {
		m_apiContext->QuerySwapChain()->Swap();
	}

	void VirtualRenderPipeline::SetViewport(Int32 width, Int32 height) {
		m_localViewport = Viewport(width, height);
	}

	void VirtualRenderPipeline::ClearTargets(const RenderResourcesStorage& storage, BatchSlot slot) {
		storage.HandleResources<ITargetResourceData>(slot, [&](ITargetResourceData* resource) { resource->Clear(m_apiContext); });
	}

	void VirtualRenderPipeline::SetDepthStencilCleaningFlags(ITargetResourceData* resource, bool enableDepthCleaning, bool enableStencilCleaning, Int32 stencilValue) {
		if (resource->IsDepth()) {
			IDepthStencilResourceData* depthResource = dynamic_cast<IDepthStencilResourceData*>(resource);
			depthResource->EnableDepthClear(enableDepthCleaning);
			depthResource->EnableStencilClear(enableStencilCleaning);
			depthResource->SetStencilClearValue(stencilValue);
		}
	}

	void VirtualRenderPipeline::UpdateBuffer(IBufferResourceData* resource, std::function<void(RawData&)> updater) {
		IDynamicResourceData* dynamicResource = dynamic_cast<IDynamicResourceData*>(resource);

		updater(dynamicResource->GetBufferData());
		dynamicResource->Update(m_apiContext);
	}

	Int32 VirtualRenderPipeline::GetRenderSpaceWidth() const {
		return m_localViewport.GetWidth();
	}

	Int32 VirtualRenderPipeline::GetRenderSpaceHeight() const {
		return m_localViewport.GetHeight();
	}

	ITargetResourceData* VirtualRenderPipeline::GetTarget() const {
		return m_apiContext->QuerySwapChain()->GetOutputTarget();
	}

	template<>
	void VirtualRenderPipeline::AddResource(RenderStage stage, IBufferResourceData* resource) {
		Array<IBufferResourceData*>& temp =  m_localStages[static_cast<Int32>(stage)].m_localBuffers;
		temp.push_back(resource);
	}

	template<>
	void VirtualRenderPipeline::AddResource(RenderStage stage, ITextureResourceData* resource) {
		Array<ITextureResourceData*>& temp = m_localStages[static_cast<Int32>(stage)].m_localTextures;
		temp.push_back(resource);
	}

	template<>
	void VirtualRenderPipeline::AddResource(RenderStage stage, IStateResourceData* resource) {
		Array<IStateResourceData*>& temp = m_localStages[static_cast<Int32>(stage)].m_localSamplers;
		temp.push_back(resource);
	}

	template<>
	void VirtualRenderPipeline::AddResource(RenderStage stage, IShaderResourceData* resource) {
		m_localStages[static_cast<Int32>(stage)].m_localShader = resource;
	}

	template<>
	void VirtualRenderPipeline::AddResource(ITargetResourceData* resource) {
		m_localGBuffer.push_back(resource);
	}

	template<>
	void VirtualRenderPipeline::AddResource(IStateResourceData* resource) {
		m_localStates.push_back(resource);
	}

	template<>
	void VirtualRenderPipeline::AddResources<IBufferResourceData>(const RenderResourcesStorage& storage, BatchSlot slot, RenderStage stage) {
		storage.HandleResources<IBufferResourceData>(slot, [&](IBufferResourceData* resource) { AddResource(stage, resource); });
	}

	template<>
	void VirtualRenderPipeline::AddResources<ITextureResourceData>(const RenderResourcesStorage& storage, BatchSlot slot, RenderStage stage) {
		storage.HandleResources<ITextureResourceData>(slot, [&](ITextureResourceData* resource) { AddResource(stage, resource); });
	}

	template<>
	void VirtualRenderPipeline::AddResources<IStateResourceData>(const RenderResourcesStorage& storage, BatchSlot slot, RenderStage stage) {
		storage.HandleResources<IStateResourceData>(slot, [&](IStateResourceData* resource) { if (resource->Is(StateType::ST_SAMPLER)) { AddResource(stage, resource); } });
	}

	template<>
	void VirtualRenderPipeline::AddResources<ITargetResourceData>(const RenderResourcesStorage& storage, BatchSlot slot) {
		storage.HandleResources<ITargetResourceData>(slot, [&](ITargetResourceData* resource) { AddResource(resource); });
	}

	template<>
	void VirtualRenderPipeline::AddResources<IStateResourceData>(const RenderResourcesStorage& storage, BatchSlot slot) {
		storage.HandleResources<IStateResourceData>(slot, [&](IStateResourceData* resource) { if (!resource->Is(StateType::ST_SAMPLER)) { AddResource(resource); } });
	}

	template<>
	void VirtualRenderPipeline::AddResources<IBufferResourceData>(RenderStage stage, const Array<IBufferResourceData*>& resources) {
		Array<IBufferResourceData*>& localBuffers = m_localStages[static_cast<Int32>(stage)].m_localBuffers;
		localBuffers.insert(localBuffers.end(), resources.begin(), resources.end());
	}

	template<>
	void VirtualRenderPipeline::AddResources<ITextureResourceData>(RenderStage stage, const Array<ITextureResourceData*>& resources) {
		Array<ITextureResourceData*>& localTextures = m_localStages[static_cast<Int32>(stage)].m_localTextures;
		localTextures.insert(localTextures.end(), resources.begin(), resources.end());
	}

	template<>
	void VirtualRenderPipeline::AddResources<IStateResourceData>(RenderStage stage, const Array<IStateResourceData*>& resources) {
		Array<IStateResourceData*>& localSamplers = m_localStages[static_cast<Int32>(stage)].m_localSamplers;
		localSamplers.insert(localSamplers.end(), resources.begin(), resources.end());
	}

	template<>
	void VirtualRenderPipeline::AddResources<IStateResourceData>(const Array<IStateResourceData*>& resources) {
		m_localStates.insert(m_localStates.end(), resources.begin(), resources.end());
	}

	template<>
	void VirtualRenderPipeline::AddResources<ITargetResourceData>(const Array<ITargetResourceData*>& resources) {
		m_localGBuffer.insert(m_localGBuffer.end(), resources.begin(), resources.end());
	}

	void VirtualRenderPipeline::AddResourcesFromTargetsToTextures(const RenderResourcesStorage& storage, BatchSlot slot, RenderStage stage) {
		storage.HandleResources<ITargetResourceData>(slot, [&](ITargetResourceData* resource) { AddResource(stage, resource->GetTextureResource()); });
	}

	inline void VirtualRenderPipeline::AdjustViewport() {
		Viewport viewport;
		m_apiContext->QueryPipeline()->GetViewport(viewport);

		if (viewport != m_localViewport) {
			m_apiContext->QueryPipeline()->SetViewport(m_localViewport.GetWidth(), m_localViewport.GetHeight());
		}
	}

	template<typename T>
	inline constexpr void BindResources(Array<T*>& resources, std::function<void(const Array<T*>&)> callback) {
		if (resources.size() > 0) {
			callback(resources);
			resources.clear();
		}
	}

	void VirtualRenderPipeline::SetApiPipeline() {
		IRenderPipeline* apiPipeline = m_apiContext->QueryPipeline();

		BindResources<ITargetResourceData>(m_localGBuffer, [&](const Array<ITargetResourceData*>& resources) { apiPipeline->SetTargets(resources); });
		BindResources<IStateResourceData>(m_localStates, [&](const Array<IStateResourceData*>& resources) { apiPipeline->SetStates(resources); });

		for (Int32 i = 0; i < m_localStages.size(); i++) {
			IRenderStage* stage = apiPipeline->GetStage(static_cast<RenderStage>(i));
			
			if (m_localStages[i].m_localShader != nullptr) {
				BindResources<ITextureResourceData>(m_localStages[i].m_localTextures, [&](const Array<ITextureResourceData*>& resources) { stage->BindTextures(resources); });
				BindResources<IBufferResourceData>(m_localStages[i].m_localBuffers, [&](const Array<IBufferResourceData*>& resources) { stage->BindBuffers(resources); });
				BindResources<IStateResourceData>(m_localStages[i].m_localSamplers, [&](const Array<IStateResourceData*>& resources) { stage->BindSamplers(resources); });
			}
			stage->BindShader(m_localStages[i].m_localShader);
		}
	}
}
