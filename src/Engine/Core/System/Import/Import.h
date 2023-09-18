#ifndef IMPORT_H
#define IMPORT_H

#include "Engine/Object/Object.h"
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

    class Importer {
    private:
        const String m_filename;

    public:
        Importer(const String& filename);
        virtual ~Importer() = default;

        const String& GetFilename() const;

        virtual void ImportTo(Object* object);
    };
}

#endif // IMPORT_H
