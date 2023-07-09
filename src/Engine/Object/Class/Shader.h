#ifndef SHADER_H
#define SHADER_H

#include "Engine/Object/Object.h"

namespace Engine {
    CLASSTYPE(Shader)
    class Shader : public Object {
        GENERATE_BODY(Shader, Object)

    public:
        Shader(const ObjectArgument& argument);
        Shader(const Shader&) = default;
        virtual ~Shader() = default;
    };
}
#endif // SHADER_H
