#include "Engine/Object/ObjectArgument.h"

namespace Engine {
    ObjectArgumentPack::ObjectArgumentPack(const void* data, Size size) {
        argument = new UInt8[size];
        argumentSize = size;

        memcpy_s(argument, size, data, size);
    }

    ObjectArgumentPack::ObjectArgumentPack(const ObjectArgumentPack& pack)
        : ObjectArgumentPack(pack.argument, pack.argumentSize) {

    }

    ObjectArgumentPack::~ObjectArgumentPack() {
        delete[] static_cast<UInt8*>(argument);
    }

    const ObjectArgument& ObjectArgument::Dummy() {
        static ObjectArgument argument;
        return argument;
    }
}
