#include "../public/Type.h"
#include "../public/Property.h"
#include "../public/Method.h"

namespace DecaVerseCore
{
    Type::Type(const std::string& name, TypeId id, size_t size, Type* parent)
        : m_name(name)
        , m_typeId(id)
        , m_size(size)
        , m_parent(parent)
        , m_factory(nullptr)
    {
    }

    bool Type::IsA(TypeId typeId) const
    {
        if (m_typeId == typeId)
            return true;

        if (m_parent)
            return m_parent->IsA(typeId);

        return false;
    }

    bool Type::IsA(const std::string& typeName) const
    {
        TypeId id = GenerateTypeId(typeName);
        return IsA(id);
    }

    bool Type::IsChildOf(const Type* type) const
    {
        if (!type)
            return false;

        return IsA(type->GetTypeId());
    }

    void Type::AddProperty(std::shared_ptr<Property> property)
    {
        if (property)
        {
            m_properties.push_back(property);
            m_propertyMap[property->GetName()] = property.get();
        }
    }

    Property* Type::GetProperty(const std::string& name) const
    {
        auto it = m_propertyMap.find(name);
        if (it != m_propertyMap.end())
            return it->second;
        return nullptr;
    }

    void Type::AddMethod(std::shared_ptr<Method> method)
    {
        if (method)
        {
            m_methods.push_back(method);
            m_methodMap[method->GetName()] = method.get();
        }
    }

    Method* Type::GetMethod(const std::string& name) const
    {
        auto it = m_methodMap.find(name);
        if (it != m_methodMap.end())
            return it->second;
        return nullptr;
    }

    void* Type::CreateInstance() const
    {
        if (m_factory)
            return m_factory();
        return nullptr;
    }
}
