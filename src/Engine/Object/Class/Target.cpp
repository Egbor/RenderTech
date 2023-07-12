#include "Engine/Object/Class/Target.h"
#include "Engine/Core/System/Exception/EngineException.h"

namespace Engine {
    GENERATE_INSTANTIATION(Target)

    Target::Target(const ObjectArgument& argument)
        : Super(argument) {

    }

    Texture2D* Target::GetTexture2D() const {
        throw new EngineException("[Target] Target::GetTexture2D() is abstract method without any features");
    }

    void Target::Clear(TargetClear clear) {
        throw new EngineException("[Target] Target::Clear() is abstract method without any features");
    }
}
