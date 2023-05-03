#include "Engine/Rendering/RenderPass/ContentRenderPass.h"

namespace Engine {
    ContentRenderPass::ContentRenderPass(Context* context, Texture2D* target)
        : RenderPass(context), m_context(context), 
        m_target(target), m_binders(5), m_states(4) {
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

    ContentRenderPass::~ContentRenderPass() {
        DELETE_ARRAY_OF_OBJECTS(m_states);
        DELETE_ARRAY_OF_OBJECTS(m_binders);
    }

    void ContentRenderPass::Render(RenderPass* prev) {

    }

    void ContentRenderPass::BindRenderStates() {
        for (RenderState* state : m_states) {
            state->Bind();
        }
    }

    void ContentRenderPass::UnBindRenderStates() {
        for (RenderState* state : m_states) {
            state->UnBind();
        }
    }

    void ContentRenderPass::BindRenderResources(ResourceType type) {
        m_binders[static_cast<Int32>(type)]->Bind();
    }

    void ContentRenderPass::UnBindRenderResources(ResourceType type) {
        m_binders[static_cast<Int32>(type)]->UnBind();
    }

    Context* ContentRenderPass::GetContext() const {
        return m_context;
    }

    Texture2D* ContentRenderPass::GetViewportTarget() const {
        return m_target;
    }
}