#ifndef RENDERPASS_H
#define RENDERPASS_H

#include "Engine/Rendering/Utils/TargetMap.h"

namespace Engine {
    class RenderPass {
    private:
        TargetMap* m_targets;

    public:
        RenderPass(Context* context);
        virtual ~RenderPass();

        Target* GetTarget(RenderOutput slot) const;
        void ClearTarget(RenderOutput slot, TargetClear clear);

        virtual void Render(RenderPass* prev);

    protected:
        void AddTarget(RenderOutput slot, Target* target);
        void AddTarget(RenderOutput slot, Texture2D* texture);
        void AddTarget(RenderOutput slot, TextureFormat format, UInt32 width, UInt32 height);
        void RemoveTarget(RenderOutput slot);
    };
}

#endif // RENDERPASS_H
