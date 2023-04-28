#include "Engine/Object/Class/Buffer.h"
#include "Engine/Core/System/Exception/EngineException.h"

namespace Engine {
    GENERATE_RTTI_DEFINITIONS(Buffer)

    Buffer::Buffer(const ObjectArgument& argument)
        : Super(argument) {

    }

    UInt32 Buffer::GetBufferNumBytes() const {
        throw new EngineException("[Buffer] Buffer::GetBufferNumBytes() is abstract method without any features");
    }

    UInt32 Buffer::GetBufferNumElements() const {
        throw new EngineException("[Buffer] Buffer::GetBufferNumElements() is abstract method without any features");
    }

    GENERATE_RTTI_DEFINITIONS(DynamicBuffer)

    DynamicBuffer::DynamicBuffer(const ObjectArgument& argument)
        : Super(argument) {

    }

    void DynamicBuffer::Update(const BufferInfo& info) {
        throw new EngineException("[DynamicBuffer] DynamicBuffer::Update() is abstract method without any features");
    }
}
