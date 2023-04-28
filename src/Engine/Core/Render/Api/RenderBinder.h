#ifndef RENDERBINDER_H
#define RENDERBINDER_H

#include "Engine/Core/Render/Api/RenderDef.h"

namespace Engine {
    class RenderBinderBase {
    public:
        RenderBinderBase() = default;
        RenderBinderBase(const RenderBinderBase&) = default;
        virtual ~RenderBinderBase() = default;

        virtual void Bind() = 0;
        virtual void UnBind() = 0;

        virtual void AttachRenderItem(Int32 id, void* item) = 0;
        virtual void DetachRenderItem(Int32 id) = 0;
    };

    template<class TEnumClass, class TRenderClass>
    class RenderBinder : public RenderBinderBase {
    public:
        RenderBinder() = default;
        RenderBinder(const RenderBinder&) = default;
        virtual ~RenderBinder() = default;

        virtual void Bind() override {};
        virtual void UnBind() override {};

        virtual void AttachRenderItem(TEnumClass id, TRenderClass* item) {};
        virtual void DetachRenderItem(TEnumClass id) {};

    protected:
        virtual void AttachRenderItem(Int32 id, void* item) override {
            AttachRenderItem(static_cast<TEnumClass>(id), reinterpret_cast<TRenderClass*>(item));
        }

        virtual void DetachRenderItem(Int32 id) override {
            DetachRenderItem(static_cast<TEnumClass>(id));
        }
    };
}

#endif // RENDERBINDER_H
