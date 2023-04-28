#ifndef BUFFER_H
#define BUFFER_H

#include "Engine/Core/Render/Api/RenderDef.h"
#include "Engine/Object/Object.h"

namespace Engine {
    class Buffer : public Object {
        GENERATE_BODY(Buffer, Object)

    public:
        Buffer(const ObjectArgument& argument);
        Buffer(const Buffer&) = default;
        virtual ~Buffer() = default;

        virtual UInt32 GetBufferNumBytes() const;
        virtual UInt32 GetBufferNumElements() const;
    };

    class DynamicBuffer : public Buffer {
        GENERATE_BODY(DynamicBuffer, Buffer)

    public:
        DynamicBuffer(const ObjectArgument& argument);
        DynamicBuffer(const DynamicBuffer&) = default;
        virtual ~DynamicBuffer() = default;

        virtual void Update(const BufferInfo& info);
    };
}
#endif // BUFFER_H
