#ifndef BLENDSTATE_H
#define BLENDSTATE_H

#include "Engine/Core/Render/Api/RenderDef.h"
#include "Engine/Core/Render/Api/State/RenderState.h"

namespace Engine {
    enum class Blend {
        B_ZERO = 0,
        B_ONE = 1,
        B_SRC_COLOR = 2,
        B_INV_SRC_COLOR = 3,
        B_SRC_ALPHA = 4,
        B_INV_SRC_ALPHA = 5,
        B_DEST_ALPHA = 6,
        B_INV_DEST_ALPHA = 7,
        B_DEST_COLOR = 8,
        B_INV_DEST_COLOR = 9,
        B_SRC_ALPHA_SAT = 10,
        B_BLEND_FACTOR = 11,
        B_INV_BLEND_FACTOR = 12,
        B_SRC1_COLOR = 13,
        B_INV_SRC1_COLOR = 14,
        B_SRC1_ALPHA = 15,
        B_INV_SRC1_ALPHA = 16
    };

    enum class BlendOperation {
        BO_ADD = 0,
        BO_SUBTRACT = 1,
        BO_REV_SUBTRACT = 2,
        BO_MIN = 3,
        BO_MAX = 4
    };

    class BlendState : public RenderState {
    public:
        BlendState() = default;
        BlendState(const BlendState&) = default;
        virtual ~BlendState() = default;

        virtual void SetSourceFactor(RenderOutput target, Blend factor) = 0;
        virtual void SetDestinationFactor(RenderOutput target, Blend factor) = 0;
        virtual void SetOperation(RenderOutput target, BlendOperation operation) = 0;

        virtual void SetEnable(RenderOutput target, bool enable) = 0;

       static StateType Type() {
            return StateType::ST_BLEND;
        }
    };
}

#endif // BLENDSTATE_H
