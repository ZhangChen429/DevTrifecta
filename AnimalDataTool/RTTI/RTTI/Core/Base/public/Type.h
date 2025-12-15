#pragma once
#include "DecaVerseTypeId.h"
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>

namespace DecaVerseCore
{
    // 前向声明
    class Property;
    class Method;

    // Type 类 - 核心类型信息
    class Type
    {
    public:
        Type(const std::string& name, TypeId id, size_t size, Type* parent = nullptr);
        ~Type() = default;

        // 基本信息获取
        const std::string& GetName() const { return m_name; }
        TypeId GetTypeId() const { return m_typeId; }
        size_t GetSize() const { return m_size; }
        Type* GetParent() const { return m_parent; }

        // 继承关系检查
        bool IsA(TypeId typeId) const;
        bool IsA(const std::string& typeName) const;
        bool IsChildOf(const Type* type) const;

        // 属性管理
        void AddProperty(std::shared_ptr<Property> property);
        Property* GetProperty(const std::string& name) const;
        const std::vector<std::shared_ptr<Property>>& GetProperties() const { return m_properties; }

        // 方法管理
        void AddMethod(std::shared_ptr<Method> method);
        Method* GetMethod(const std::string& name) const;
        const std::vector<std::shared_ptr<Method>>& GetMethods() const { return m_methods; }

        // 对象创建（需要派生类实现具体的工厂函数）
        using FactoryFunction = void* (*)();
        void SetFactory(FactoryFunction factory) { m_factory = factory; }
        void* CreateInstance() const;

    private:
        std::string m_name;
        TypeId m_typeId;
        size_t m_size;
        Type* m_parent;

        std::vector<std::shared_ptr<Property>> m_properties;
        std::unordered_map<std::string, Property*> m_propertyMap;

        std::vector<std::shared_ptr<Method>> m_methods;
        std::unordered_map<std::string, Method*> m_methodMap;

        FactoryFunction m_factory;
    };
}
