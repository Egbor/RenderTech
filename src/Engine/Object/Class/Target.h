#ifndef TARGET_H
#define TARGET_H

#include "Engine/Object/Class/Texture.h"

namespace Engine {
    struct TargetClear {
        union {
            struct {
                Float rgba[4];
            } forTarget;
            struct {
                Float depth;
                Int32 stencil;
                bool clearDepth;
                bool clearStencil;
            } forDepthStencil;
        };
    };

    class Target : public Object {
        GENERATE_BODY(Target, Object)

    public:
        Target(const ObjectArgument& argument);
        Target(const Target&) = default;
        virtual ~Target() = default;

        virtual Texture2D* GetTexture2D() const;
        virtual void Clear(TargetClear clear);
    };
}

#endif // TARGET_H
