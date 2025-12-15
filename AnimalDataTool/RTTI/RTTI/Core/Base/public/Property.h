#pragma once
#include "DecaVerseTypeId.h"
#include <string>
#include <functional>

namespace DecaVerseCore
{
    // Any 类型用于存储任意类型的值
    class Any
    {
    public:
        Any() : m_data(nullptr), m_typeId(0) {}

        template<typename T>
        Any(const T& value) : m_typeId(GenerateTypeId(typeid(T).name()))
        {
            m_data = new T(value);
        }

        ~Any()
        {
            if (m_data)
                delete static_cast<char*>(m_data);
        }

        template<typename T>
        T* As()
        {
            if (m_typeId == GenerateTypeId(typeid(T).name()))
                return static_cast<T*>(m_data);
            return nullptr;
        }

        template<typename T>
        const T* As() const
        {
            if (m_typeId == GenerateTypeId(typeid(T).name()))
                return static_cast<const T*>(m_data);
            return nullptr;
        }

        TypeId GetTypeId() const { return m_typeId; }
        bool IsValid() const { return m_data != nullptr; }

    private:
        void* m_data;
        TypeId m_typeId;
    };

    // Property 类 - 属性反射
    class Property
    {
    public:
        using Getter = std::function<Any(void*)>;
        using Setter = std::function<void(void*, const Any&)>;

        Property(const std::string& name, TypeId typeId, size_t offset, Getter getter, Setter setter)
            : m_name(name)
            , m_typeId(typeId)
            , m_offset(offset)
            , m_getter(getter)
            , m_setter(setter)
        {
        }

        const std::string& GetName() const { return m_name; }
        TypeId GetTypeId() const { return m_typeId; }
        size_t GetOffset() const { return m_offset; }

        Any GetValue(void* instance) const
        {
            if (m_getter)
                return m_getter(instance);
            return Any();
        }

        void SetValue(void* instance, const Any& value) const
        {
            if (m_setter)
                m_setter(instance, value);
        }

        template<typename ClassType, typename ValueType>
        static Getter CreateGetter(ValueType ClassType::*memberPtr)
        {
            return [memberPtr](void* instance) -> Any
            {
                ClassType* obj = static_cast<ClassType*>(instance);
                return Any(obj->*memberPtr);
            };
        }

        template<typename ClassType, typename ValueType>
        static Setter CreateSetter(ValueType ClassType::*memberPtr)
        {
            return [memberPtr](void* instance, const Any& value)
            {
                ClassType* obj = static_cast<ClassType*>(instance);
                const ValueType* val = value.As<ValueType>();
                if (val)
                    obj->*memberPtr = *val;
            };
        }

    private:
        std::string m_name;
        TypeId m_typeId;
        size_t m_offset;
        Getter m_getter;
        Setter m_setter;
    };
}
