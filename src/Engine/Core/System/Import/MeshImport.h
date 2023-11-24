#ifndef MESHIMPORT_H
#define MESHIMPORT_H

#include "Engine/Core/System/Import/Import.h"
#include "Engine/Object/Class/Mesh.h"

namespace Engine {
    class MeshImporter {
    public:
        static void ImportTo(const String& filename, Object* object);
    };
}

#endif // MESHIMPORT_H
