#include "Engine/Core/System/Import/ShaderImport.h"
#include <fstream>

namespace Engine {
    ShaderLoader::ShaderLoader(IRenderResourceFactory* factory, ShaderType type, const String& filename)
        : AbstractLoader(filename), m_factory(factory), m_loadType(type) {

    }

    IShaderResourceData* ShaderLoader::LoadShader() {
        return reinterpret_cast<IShaderResourceData*>(Load());
    }

    void* ShaderLoader::Load() {
        std::ifstream in(GetFilename(), std::ios::binary);

        if (!in.is_open()) {
            throw new EngineException("[ShaderImport] LoadResource() can't open the file");
        }

        in.seekg(0, std::ios::end);
        Size codeLength = in.tellg();
        in.seekg(0, std::ios::beg);

        Array<char> code(codeLength);
        in.read(code.data(), codeLength);

        return m_factory->CreateShader(m_loadType, codeLength, code.data());
    }

    //ShaderImport::ShaderImport(const String& filename)
    //    : Import(filename) {
    //}

    //ShaderDescription* ShaderImport::LoadResource() {
    //    std::ifstream in(GetImportingFilename(), std::ios::binary);

    //    if (!in.is_open()) {
    //        throw new EngineException("[ShaderImport] LoadResource() can't open the file");
    //    }

    //    ShaderDescription* shaderDesc = new ShaderDescription();
    //    ShaderHandler handler;
    //    Size length;

    //    in.seekg(0, std::ios::end);
    //    length = in.tellg();
    //    handler = shaderDesc->Create(length);
    //    in.seekg(0, std::ios::beg);
    //    in.read(handler, length);

    //    return shaderDesc;
    //}
}
