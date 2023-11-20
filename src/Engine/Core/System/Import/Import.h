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

    class LoaderBase {
    public:
        LoaderBase(const String& filename);
        virtual ~LoaderBase() = default;

        const String& GetFilename() const;

    private:
        const String m_filename;
    };

    class AbstractImporter : public LoaderBase {
    public:
        AbstractImporter(const String& filename);
        virtual ~AbstractImporter() = default;

        virtual void ImportTo(Object* object) = 0;
    };

    class AbstractLoader : public LoaderBase {      
    public:
        AbstractLoader(const String& fielname);
        virtual ~AbstractLoader() = default;

        virtual void* Load() = 0;
    };
}

#endif // IMPORT_H
