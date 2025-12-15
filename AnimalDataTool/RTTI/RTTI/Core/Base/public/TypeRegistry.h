#pragma once
#include "Type.h"
#include <unordered_map>
#include <memory>
#include <mutex>

namespace DecaVerseCore
{
    // TypeRegistry 类 - 全局类型注册表（单例模式）
    class TypeRegistry
    {
    public:
        static TypeRegistry& GetInstance()
        {
            static TypeRegistry instance;
            return instance;
        }

        // 注册类型
        void RegisterType(std::shared_ptr<Type> type)
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            if (type)
            {
                m_typesByName[type->GetName()] = type;
                m_typesById[type->GetTypeId()] = type;
            }
        }

        // 通过名称查找类型
        Type* FindTypeByName(const std::string& name) const
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            auto it = m_typesByName.find(name);
            if (it != m_typesByName.end())
                return it->second.get();
            return nullptr;
        }

        // 通过 ID 查找类型
        Type* FindTypeById(TypeId id) const
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            auto it = m_typesById.find(id);
            if (it != m_typesById.end())
                return it->second.get();
            return nullptr;
        }

        // 获取所有类型
        std::vector<Type*> GetAllTypes() const
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            std::vector<Type*> types;
            types.reserve(m_typesById.size());
            for (const auto& pair : m_typesById)
                types.push_back(pair.second.get());
            return types;
        }

        // 类型统计
        size_t GetTypeCount() const
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            return m_typesById.size();
        }

    private:
        TypeRegistry() = default;
        ~TypeRegistry() = default;
        TypeRegistry(const TypeRegistry&) = delete;
        TypeRegistry& operator=(const TypeRegistry&) = delete;

        mutable std::mutex m_mutex;
        std::unordered_map<std::string, std::shared_ptr<Type>> m_typesByName;
        std::unordered_map<TypeId, std::shared_ptr<Type>> m_typesById;
    };
}
