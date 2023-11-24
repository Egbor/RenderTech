#ifndef TEXTUREIMPORT_H
#define TEXTUREIMPORT_H

#include "Engine/Object/Class/Texture.h"

namespace Engine {
    class TextureImporter {
    public:
        static void ImportTo(const String& filename, Object* object);
    };
}

#endif // TEXTUREIMPORT_H
