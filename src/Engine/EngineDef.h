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
    template<typename TType>
    struct TypeName { static const std::string name; };

#define NONEGENERICTYPE(Type)                                                           \
    template<>                                                                          \
    const String TypeName<Type>::name = #Type;                                          \

#define GENERICTYPE(Type)                                                               \
    template<typename TType>                                                            \
    struct TypeName<Type<TType>> {                                                      \
        inline static const String name = #Type + "<" + TypeName<TType>::name + ">";    \
    };

#define VALUETYPE(BaseType, Type, Generic) using Type = BaseType; Generic##GENERICTYPE(Type)
#define CLASSTYPE(Type) class Type; NONEGENERICTYPE(Type)
#define STRUCTTYPE(Type) struct Type; NONEGENERICTYPE(Type)

    VALUETYPE(std::string, String, NONE)//using String = std::string;
    VALUETYPE(std::wstring, WString, NONE)//using WString = std::wstring;

    VALUETYPE(float, Float, NONE)//typedef float Float;
    VALUETYPE(double, Double, NONE)//typedef double Double;

    VALUETYPE(signed char, Int8, NONE)//typedef signed char Int8;
    VALUETYPE(signed short, Int16, NONE)//typedef signed short Int16;
    VALUETYPE(signed int, Int32, NONE)//typedef signed int Int32;
    VALUETYPE(signed long long, Int64, NONE)//typedef signed long long Int64;

    VALUETYPE(unsigned char, UInt8, NONE)//typedef unsigned char UInt8;
    VALUETYPE(unsigned short, UInt16, NONE)//typedef unsigned short UInt16;
    VALUETYPE(unsigned int, UInt32, NONE)//typedef unsigned int UInt32;
    VALUETYPE(unsigned long long, UInt64, NONE)//typedef unsigned long long UInt64;

    typedef size_t Size;
    typedef void* HandlerPtr;

    template<typename TKey, typename T>
    using Map = typename std::unordered_map<TKey, T>;
    template<typename T>
    VALUETYPE(typename std::vector<T>, Array)//using Array = typename std::vector<T>;
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
