#ifndef ENGINEDEF_H
#define ENGINEDEF_H

#include <string>
#include <vector>
#include <list>
#include <unordered_map>
#include <memory>
#include <functional>

#include "Engine/Core/System/Input/InputDef.h"

namespace Engine {
    typedef float Float;
    typedef double Double;

    typedef signed char Int8;
    typedef signed short Int16;
    typedef signed int Int32;
    typedef signed long long Int64;

    typedef unsigned char UInt8;
    typedef unsigned short UInt16;
    typedef unsigned int UInt32;
    typedef unsigned long long UInt64;

    typedef size_t Size;
    typedef void* HandlerPtr;

    using String = std::string;
    using WString = std::wstring;

    template<typename TKey, typename T>
    using Map = typename std::unordered_map<TKey, T>;
    template<typename T>
    using Array = typename std::vector<T>;
    template<typename T>
    using List = typename std::list<T>;

    template<typename TKey, typename T>
    using MapIterator = typename Map<TKey, T>::iterator;
    template<typename T>
    using ArrayIterator = typename Array<T>::iterator;
    template<typename T>
    using ListIterator = typename List<T>::iterator;

    template<typename T>
    using ConstListIterator = typename List<T>::const_iterator;

#define DELETE_OBJECT(obj) if ((obj) != nullptr) { delete (obj); (obj) = nullptr; }
#define DELETE_ARRAY_OF_OBJECTS(arr) for (auto obj : (arr)) { DELETE_OBJECT(obj); }

#define FIELD_AS_TEXT(field) #field
#define IS_POWER_OF_TWO(x) (x && (!(x&(x-1))))

}

#endif // ENGINEDEF_H
