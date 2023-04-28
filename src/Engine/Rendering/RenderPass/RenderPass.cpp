#include "Engine/Rendering/RenderPass/RenderPass.h"
#include "Engine/Core/System/Exception/EngineException.h"

namespace Engine {
    RenderPass::RenderPass(Context* context, SwapChain* swapchain)
        : m_context(context), m_swapchain(swapchain), m_binders(5),
          m_states(4), m_targets(RENDER_OUTPUT_COUNT) {
        m_binders[static_cast<Int32>(ResourceType::RB_TEXTURE)] = context->CreateBinder(tagREALTIME_TEXTURE_BINDER);
        m_binders[static_cast<Int32>(ResourceType::RB_DYNAMIC)] = context->CreateBinder(tagREALTIME_DYNAMIC_BUFFER_BINDER);
        m_binders[static_cast<Int32>(ResourceType::RB_TARGET)] = context->CreateBinder(tagREALTIME_TARGET_BINDER);
        m_binders[static_cast<Int32>(ResourceType::RB_BUFFER)] = context->CreateBinder(tagREALTIME_BUFFER_BINDER);
        m_binders[static_cast<Int32>(ResourceType::RB_SHADER)] = context->CreateBinder(tagREALTIME_SHADER_BINDER);

        m_states[static_cast<Int32>(StateType::ST_BLEND)] = context->CreateState(tagREALTIME_BLEND_STATE);
        m_states[static_cast<Int32>(StateType::ST_SAMPLER)] = context->CreateState(tagREALTIME_SAMPLER_STATE);
        m_states[static_cast<Int32>(StateType::ST_DEPTH_STENCIL)] = context->CreateState(tagREALTIME_DEPTHSTENCIL_STATE);
        m_states[static_cast<Int32>(StateType::ST_RASTERIZER)] = context->CreateState(tagREALTIME_RASTERIZER_STATE);
    }

    RenderPass::~RenderPass() {
        DELETE_ARRAY_OF_OBJECTS(m_states);
        DELETE_ARRAY_OF_OBJECTS(m_binders);
        DELETE_ARRAY_OF_OBJECTS(m_targets);
    }

    void RenderPass::Render(RenderPass* prev) {
        throw EngineException("[RenderPass] RenderPass::Render() is base method without any features");
    }

    void RenderPass::AddTarget() {
        AddTarget(RenderOutput::RO_TARGET0, m_swapchain->GetBackBuffer());
    }

    void RenderPass::AddTarget(RenderOutput slot, Target* target) {
        Texture2D* texture = m_context->Copy(target->GetTexture2D());
        AddTarget(slot, texture);
        DELETE_OBJECT(texture);
    }

    void RenderPass::AddTarget(RenderOutput slot, Texture2D* texture) {
        Int32 id = static_cast<Int32>(slot);
        DELETE_OBJECT(m_targets[id]);
        m_targets[id] = m_context->CreateTarget(texture->GetTag(), texture);
    }

    void RenderPass::AddTarget(RenderOutput slot, TextureFormat format) {
        AddTarget(slot, format, m_swapchain->GetWidth(), m_swapchain->GetHeight());
    }

    void RenderPass::AddTarget(RenderOutput slot, TextureFormat format, UInt32 width, UInt32 height) {
        TextureInfo info;
        info.width = width;
        info.height = height;
        info.format = format;

        String tag;
        if (slot == RenderOutput::RO_DEPTH) {
            tag = tagDepthStencilTargetTexture;
        } else {
            tag = tagTargetTexture;
        }

        Texture2D* texture = m_context->CreateTexture2D(tag, info);
        AddTarget(slot, texture);
        DELETE_OBJECT(texture);
    }

    void RenderPass::ClearTarget(RenderOutput slot, TargetClear clear) {
        m_targets[static_cast<Int32>(slot)]->Clear(clear);
    }

    void RenderPass::RemoveTarget(RenderOutput slot) {
        DELETE_OBJECT(m_targets[static_cast<Int32>(slot)]);
    }

    void RenderPass::BindRenderStates() {
        for (RenderState* state : m_states) {
            state->Bind();
        }
    }

    void RenderPass::UnBindRenderStates() {
        for (RenderState* state : m_states) {
            state->UnBind();
        }
    }

    void RenderPass::BindRenderResources(ResourceType type) {
        m_binders[static_cast<Int32>(type)]->Bind();
    }

    void RenderPass::UnBindRenderResources(ResourceType type) {
        m_binders[static_cast<Int32>(type)]->UnBind();
    }

    Target* RenderPass::GetTarget(RenderOutput slot) const {
        return m_targets[static_cast<Int32>(slot)];
    }

    Context* RenderPass::GetContext() const {
        return m_context;
    }

    SwapChain* RenderPass::GetSwapChain() const {
        return m_swapchain;
    }
}
