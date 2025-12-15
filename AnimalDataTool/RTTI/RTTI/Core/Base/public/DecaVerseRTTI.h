#pragma once

// DecaVerse RTTI Framework - Main Header
// 包含所有 RTTI 相关的头文件

#include "DecaVerseTypeId.h"
#include "Type.h"
#include "Property.h"
#include "Method.h"
#include "TypeRegistry.h"
#include "RTTIMacros.h"

namespace DecaVerse
{
    // 便捷的类型查询函数
    inline DecaVerseCore::Type* FindType(const std::string& typeName)
    {
        return DecaVerseCore::TypeRegistry::GetInstance().FindTypeByName(typeName);
    }

    inline DecaVerseCore::Type* FindType(TypeId typeId)
    {
        return DecaVerseCore::TypeRegistry::GetInstance().FindTypeById(typeId);
    }

    // 便捷的对象创建函数
    template<typename T>
    T* CreateInstance(const std::string& typeName)
    {
        auto* type = FindType(typeName);
        if (type)
        {
            void* instance = type->CreateInstance();
            return static_cast<T*>(instance);
        }
        return nullptr;
    }
}
