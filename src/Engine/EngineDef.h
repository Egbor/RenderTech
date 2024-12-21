#ifndef ENGINEDEF_H
#define ENGINEDEF_H

#include <string>
#include <vector>
#include <queue>
#include <list>
#include <unordered_map>
#include <memory>
#include <functional>

#include <boost/uuid.hpp>
#include <boost/algorithm/string.hpp>

namespace Engine {
    template<typename TType>
    struct TypeName { static const std::string name; };

#define TYPENAME(Type)                                                           \
    template<>                                                                          \
    const String TypeName<Type>::name = #Type;                                          \

#define GENERICTYPENAME(Type)                                                               \
    template<typename TType>                                                            \
    struct TypeName<Type<TType>> {                                                      \
        inline static const String name = #Type + ("<" + TypeName<TType>::name + ">");    \
    };

#define VALUETYPE(BaseType, Type, Generic) using Type = BaseType; Generic##TYPENAME(Type)
#define CLASSTYPE(Type, Generic) class Type; Generic##TYPENAME(Type)
#define STRUCTTYPE(Type, Generic) struct Type; Generic##TYPENAME(Type)

    VALUETYPE(std::string, String)
    VALUETYPE(std::wstring, WString)

    VALUETYPE(float, Float)
    VALUETYPE(double, Double)

    VALUETYPE(signed char, Int8)
    VALUETYPE(signed short, Int16)
    VALUETYPE(signed int, Int32)
    VALUETYPE(signed long long, Int64)

    VALUETYPE(unsigned char, UInt8)
    VALUETYPE(unsigned short, UInt16)
    VALUETYPE(unsigned int, UInt32)
    VALUETYPE(unsigned long long, UInt64)
        
    typedef size_t Size;
    typedef void* HandlerPtr;

    template<typename TKey, typename T>
    using Map = typename std::unordered_map<TKey, T>;
    template<typename T>
    VALUETYPE(typename std::vector<T>, Array, GENERIC)
    template<typename T>
    using List = typename std::list<T>;
    template<typename T>
    using Queue = typename std::queue<T>;

    template<typename TKey, typename T>
    using MapIterator = typename Map<TKey, T>::iterator;
    template<typename T>
    using ArrayIterator = typename Array<T>::iterator;
    template<typename T>
    using ListIterator = typename List<T>::iterator;

    template<typename T>
    using ConstListIterator = typename List<T>::const_iterator;

    class RawData {
    public:
        RawData(Size dataSize) noexcept 
            : m_data(new char[dataSize]), m_size(dataSize) {
            
        }

        RawData(const RawData& data) noexcept 
            : RawData(data.m_size) {
            memcpy_s(m_data, m_size, data.m_data, data.m_size);
        }

        RawData(RawData&& data) noexcept
            : m_data(data.m_data), m_size(data.m_size) {
            data.m_data = nullptr;
            data.m_size = 0;
        }

        ~RawData() noexcept {
            delete[] m_data;
        }

        RawData& operator=(const RawData& data) {
            if (this == &data) {
                return *this;
            }

            delete[] m_data;
            m_size = data.m_size;
            m_data = new char[m_size];
            memcpy_s(m_data, m_size, data.m_data, data.m_size);

            return *this;
        }

        RawData& operator=(RawData&& data) {
            if (this == &data) {
                return *this;
            }

            delete[] m_data;
            m_size = data.m_size;
            m_data = data.m_data;

            data.m_size = 0;
            data.m_data = nullptr;

            return *this;
        }

        template<typename T>
        T* As() {
            assert(m_size >= sizeof(T));
            return reinterpret_cast<T*>(m_data);
        }

        static inline const RawData& Nulldata() {
            static RawData nulldata = RawData();
            return nulldata;
        }

    private:
        constexpr RawData() noexcept
            : m_data(nullptr), m_size(0) {
        }

        char* m_data;
        Size m_size;
    };

    // const RawData RawData::nulldata = RawData();

#define DELETE_OBJECT(obj) if ((obj) != nullptr) { delete (obj); (obj) = nullptr; }
#define DELETE_ARRAY_OF_OBJECTS(arr) for (auto obj : (arr)) { DELETE_OBJECT(obj); }

#define FIELD_AS_TEXT(field) #field
#define AS_TEXT(x) #x
#define IS_POWER_OF_TWO(x) (x && (!(x&(x-1))))

#define INDEX_OF(value) static_cast<UInt32>(value)


#define NUMBER_OF_FACES_FOR_SINGLE_TEXTURE 1
#define NUMBER_OF_FACES_FOR_CUBEMAP_TEXTURE 6

}

#endif // ENGINEDEF_H
