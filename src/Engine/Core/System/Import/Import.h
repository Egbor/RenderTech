#ifndef IMPORT_H
#define IMPORT_H

#include "Engine/Core/System/Exception/EngineException.h"

namespace Engine {
    template<class TResourceType, class ...TArgs>
    class Import {
    private:
        String m_filename;

    public:
        Import(const String& filename) : m_filename(filename) {}
        Import(const Import&) = default;
        virtual ~Import() = default;

        const String& GetImportingFilename() const {
            return m_filename;
        }

        virtual TResourceType* LoadResource(TArgs ...arg) {
            throw EngineException("[Import] LoadResource() is not an implementing method");
        }
    };
}

#endif // IMPORT_H
