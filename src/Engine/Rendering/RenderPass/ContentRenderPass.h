#ifndef CONTENTRENDERPASS
#define CONTENTRENDERPASS

#include "Engine/Rendering/RenderPass/RenderPass.h"

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

	class ContentRenderPass : public RenderPass {
    private:
        Context* m_context;
        Texture2D* m_target;

        Array<RenderBinderBase*> m_binders;
        Array<RenderState*> m_states;

    public:
        ContentRenderPass(Context* context, Texture2D* target);
        virtual ~ContentRenderPass();

        void Render(RenderPass * prev) override;

    protected:
        Context* GetContext() const;
        Texture2D* GetViewportTarget() const;

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

#endif // !CONTENTRENDERPASS
