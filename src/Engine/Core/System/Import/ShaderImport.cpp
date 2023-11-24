#include "Engine/Core/System/Import/ShaderImport.h"
#include "Engine/Core/System/Exception/EngineException.h"
#include "Engine/Core/Core.h"

#include <fstream>

namespace Engine {
    IShaderResourceData* ShaderLoader::Load(const String& filename, ShaderType type) {
        std::ifstream in(filename, std::ios::binary);

        if (!in.is_open()) {
            throw new EngineException("[ShaderLoader] LoadResource() can't open the file");
        }

        in.seekg(0, std::ios::end);
        Size codeLength = in.tellg();
        in.seekg(0, std::ios::beg);

        Array<char> code(codeLength);
        in.read(code.data(), codeLength);

        IRenderResourceFactory* factory = Core::GetInstance()->GetContext()->QueryResourceFactory();
        return factory->CreateShader(type, codeLength, code.data());
    }
}
