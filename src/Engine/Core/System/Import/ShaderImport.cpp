#include "Engine/Core/System/Import/ShaderImport.h"
#include <fstream>

namespace Engine {
    ShaderImport::ShaderImport(const String& filename)
        : Import(filename) {
    }

    ShaderDescription* ShaderImport::LoadResource() {
        std::ifstream in(GetImportingFilename(), std::ios::binary);

        if (!in.is_open()) {
            throw new EngineException("[ShaderImport] LoadResource() can't open the file");
        }

        ShaderDescription* shaderDesc = new ShaderDescription();
        ShaderHandler handler;
        Size length;

        in.seekg(0, std::ios::end);
        length = in.tellg();
        handler = shaderDesc->Create(length);
        in.seekg(0, std::ios::beg);
        in.read(handler, length);

        return shaderDesc;
    }
}
