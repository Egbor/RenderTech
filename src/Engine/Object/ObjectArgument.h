#ifndef OBJECTARGUMENT_H
#define OBJECTARGUMENT_H

#include "Engine/EngineDef.h"

namespace Engine {
    constexpr static const char* argLocation = "location";
    constexpr static const char* argRotation = "rotation";

    struct ObjectArgumentPack {
        void* argument;
        Size argumentSize;

        ObjectArgumentPack(const void* data, Size size);
        ObjectArgumentPack(const ObjectArgumentPack& pack);
        virtual ~ObjectArgumentPack();
    };

    class ObjectArgument {
    private:
        Map<String, ObjectArgumentPack> m_arguments;

    public:
        ObjectArgument() : m_arguments() {};
        ObjectArgument(const ObjectArgument&) = default;
        virtual ~ObjectArgument() = default;

        template<class T>
        void Put(const String& name, const T& object) {
            m_arguments.insert({name, ObjectArgumentPack(reinterpret_cast<const void*>(&object), sizeof(T))});
        }

        template<class T>
        void Pull(const String& name, T& object) const {
            auto it = m_arguments.find(name);
            if (it != m_arguments.end()) {
                object = *(static_cast<T*>(it->second.argument));
            }
        }

        static const ObjectArgument& Dummy();
    };
}

#endif // OBJECTARGUMENT_H
