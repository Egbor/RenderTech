#ifndef RENDERPASS_H
#define RENDERPASS_H

#include "Engine/Core/Render/Api/Context.h"

#include "Engine/Core/Render/Api/State/BlendState.h"
#include "Engine/Core/Render/Api/State/SamplerState.h"
#include "Engine/Core/Render/Api/State/DepthStencilState.h"
#include "Engine/Core/Render/Api/State/RasterizerState.h"

namespace Engine {
    enum class ResourceType {
        RB_TARGET,
        RB_TEXTURE,
        RB_SHADER,
        RB_BUFFER,
        RB_DYNAMIC
    };

    class RenderPass {
    private:
        Context* m_context;
        SwapChain* m_swapchain;

        Array<RenderBinderBase*> m_binders;
        Array<RenderState*> m_states;
        Array<Target*> m_targets;

    public:
        RenderPass(Context* context, SwapChain* swapchain);
        RenderPass(const RenderPass&) = default;
        virtual ~RenderPass();

        virtual void Render(RenderPass* prev);

        Target* GetTarget(RenderOutput slot) const;
        void ClearTarget(RenderOutput slot, TargetClear clear);

    protected:
        Context* GetContext() const;
        SwapChain* GetSwapChain() const;

        void AddTarget();
        void AddTarget(RenderOutput slot, Target* target);
        void AddTarget(RenderOutput slot, Texture2D* texture);
        void AddTarget(RenderOutput slot, TextureFormat format);
        void AddTarget(RenderOutput slot, TextureFormat format, UInt32 width, UInt32 height);
        void RemoveTarget(RenderOutput slot);

        template<class TSlotEnum>
        void AttachRenderResource(ResourceType type, TSlotEnum slot, void* item) {
            m_binders[static_cast<Int32>(type)]->AttachRenderItem(static_cast<Int32>(slot), item);
        }

        template<class TSlotEnum>
        void DetachRenderResource(ResourceType type, TSlotEnum slot) {
            m_binders[static_cast<Int32>(type)]->DetachRenderItem(static_cast<Int32>(slot));
        }

        template<class TStateClass>
        TStateClass* GetRenderState() const {
            return dynamic_cast<TStateClass*>(m_states[static_cast<Int32>(TStateClass::Type())]);
        }

        void BindRenderStates();
        void UnBindRenderStates();

        void BindRenderResources(ResourceType type);
        void UnBindRenderResources(ResourceType type);
    };
}

#endif // RENDERPASS_H
