#ifndef DEPTHSTENCILSATE_H
#define DEPTHSTENCILSATE_H

#include "Engine/Core/Render/Api/State/RenderState.h"
#include "Engine/EngineDef.h"

namespace Engine {
    enum class ComparisonFunction {
        CF_NEVER = 0,
        CF_LESS = 1,
        CF_EQUAL = 2,
        CF_LESS_EQUAL = 3,
        CF_GREATER = 4,
        CF_NOT_EQUAL = 5,
        CF_GREATER_EQUAL = 6,
        CF_ALWAYS = 7
    };

    enum class StencilOperation {
        SO_KEEP = 0,
        SO_ZERO = 1,
        SO_REPLACE = 2,
        SO_INCR_SAT = 3,
        SO_DECR_SAT = 4,
        SO_INVERT = 5,
        SO_INCR = 6,
        SO_DECR = 7
    };

    struct StencilConditions {
        StencilOperation opStencilFail;
        StencilOperation opStencilDepthFail;
        StencilOperation opStencilPass;
    };

    class DepthStencilState : public RenderState {
    public:
        DepthStencilState() = default;
        DepthStencilState(const DepthStencilState&) = default;
        virtual ~DepthStencilState() = default;

        virtual void SetDepthTestComparisonFunction(ComparisonFunction func) = 0;
        virtual void SetStencilTestComparisonBackFunction(ComparisonFunction func) = 0;
        virtual void SetStencilTestComparisonFrontFunction(ComparisonFunction func) = 0;

        virtual void SetStencilTestBackOperation(StencilConditions conditions) = 0;
        virtual void SetStencilTestFrontOperation(StencilConditions conditions) = 0;

        virtual void SetDepthTestEnable(bool enable) = 0;
        virtual void SetStencilTestEnable(bool enable) = 0;

        virtual void SetDepthWriteEnable(bool enable) = 0;
        virtual void SetStencilRef(UInt32 ref) = 0;

        static StateType Type() {
            return StateType::ST_DEPTH_STENCIL;
        }
    };
}

#endif // DEPTHSTENCILSATE_H
