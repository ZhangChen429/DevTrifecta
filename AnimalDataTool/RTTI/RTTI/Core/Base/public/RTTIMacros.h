#pragma once
#include "TypeRegistry.h"

// RTTI 宏定义 - 简化 RTTI 的使用

// 在类定义中声明 RTTI 支持
#define DECAVERSE_TYPE(ClassName, ParentClass) \
public: \
    static DecaVerseCore::Type* StaticType(); \
    virtual DecaVerseCore::Type* GetType() const { return StaticType(); } \
    static const char* StaticTypeName() { return #ClassName; } \
    virtual const char* GetTypeName() const { return StaticTypeName(); } \
    static void* CreateInstance() { return new ClassName(); }

// 在类定义中声明 RTTI 支持（无父类）
#define DECAVERSE_TYPE_ROOT(ClassName) \
public: \
    static DecaVerseCore::Type* StaticType(); \
    virtual DecaVerseCore::Type* GetType() const { return StaticType(); } \
    static const char* StaticTypeName() { return #ClassName; } \
    virtual const char* GetTypeName() const { return StaticTypeName(); } \
    static void* CreateInstance() { return new ClassName(); }

// 在 CPP 文件中实现类型注册（带父类）
#define DECAVERSE_IMPLEMENT_TYPE(ClassName, ParentClass) \
    DecaVerseCore::Type* ClassName::StaticType() \
    { \
        static std::shared_ptr<DecaVerseCore::Type> type = nullptr; \
        if (!type) \
        { \
            type = std::make_shared<DecaVerseCore::Type>( \
                #ClassName, \
                GenerateTypeId(#ClassName), \
                sizeof(ClassName), \
                ParentClass::StaticType() \
            ); \
            type->SetFactory(&ClassName::CreateInstance); \
            DecaVerseCore::TypeRegistry::GetInstance().RegisterType(type); \
        } \
        return type.get(); \
    }

// 在 CPP 文件中实现类型注册（无父类）
#define DECAVERSE_IMPLEMENT_TYPE_ROOT(ClassName) \
    DecaVerseCore::Type* ClassName::StaticType() \
    { \
        static std::shared_ptr<DecaVerseCore::Type> type = nullptr; \
        if (!type) \
        { \
            type = std::make_shared<DecaVerseCore::Type>( \
                #ClassName, \
                GenerateTypeId(#ClassName), \
                sizeof(ClassName), \
                nullptr \
            ); \
            type->SetFactory(&ClassName::CreateInstance); \
            DecaVerseCore::TypeRegistry::GetInstance().RegisterType(type); \
        } \
        return type.get(); \
    }

// 注册属性
#define DECAVERSE_PROPERTY(ClassName, PropertyName, MemberVar) \
    { \
        auto prop = std::make_shared<DecaVerseCore::Property>( \
            #PropertyName, \
            GenerateTypeId(typeid(decltype(ClassName::MemberVar)).name()), \
            offsetof(ClassName, MemberVar), \
            DecaVerseCore::Property::CreateGetter(&ClassName::MemberVar), \
            DecaVerseCore::Property::CreateSetter(&ClassName::MemberVar) \
        ); \
        ClassName::StaticType()->AddProperty(prop); \
    }

// 注册方法（无参数）
#define DECAVERSE_METHOD_0(ClassName, MethodName, MethodPtr) \
    { \
        auto method = std::make_shared<DecaVerseCore::Method>( \
            #MethodName, \
            GenerateTypeId(typeid(decltype(MethodPtr)).name()), \
            DecaVerseCore::Method::CreateInvoker(MethodPtr) \
        ); \
        ClassName::StaticType()->AddMethod(method); \
    }

// 注册方法（1 个参数）
#define DECAVERSE_METHOD_1(ClassName, MethodName, MethodPtr) \
    { \
        auto method = std::make_shared<DecaVerseCore::Method>( \
            #MethodName, \
            GenerateTypeId(typeid(decltype(MethodPtr)).name()), \
            DecaVerseCore::Method::CreateInvoker(MethodPtr) \
        ); \
        ClassName::StaticType()->AddMethod(method); \
    }

// 注册方法（2 个参数）
#define DECAVERSE_METHOD_2(ClassName, MethodName, MethodPtr) \
    { \
        auto method = std::make_shared<DecaVerseCore::Method>( \
            #MethodName, \
            GenerateTypeId(typeid(decltype(MethodPtr)).name()), \
            DecaVerseCore::Method::CreateInvoker(MethodPtr) \
        ); \
        ClassName::StaticType()->AddMethod(method); \
    }

// ========================================
// UE 风格的反射宏系统
// ========================================

// 开始反射声明块 - 生成静态注册函数的开始
#define DVREFLECT_BEGIN(ClassName) \
public: \
    static void RegisterReflection() \
    { \
        using ThisClass = ClassName;

// 声明并注册属性 (UE风格)
// 用法: DVPROPERTY(Type, PropertyName, MemberVariable)
#define DVPROPERTY(Type, PropertyName, MemberVar) \
        { \
            auto prop = std::make_shared<DecaVerseCore::Property>( \
                #PropertyName, \
                GenerateTypeId(typeid(Type).name()), \
                offsetof(ThisClass, MemberVar), \
                DecaVerseCore::Property::CreateGetter(&ThisClass::MemberVar), \
                DecaVerseCore::Property::CreateSetter(&ThisClass::MemberVar) \
            ); \
            ThisClass::StaticType()->AddProperty(prop); \
        }

// 声明并注册方法 (UE风格) - 无参数
// 用法: DVFUNCTION_0(MethodName, &ClassName::MethodName)
#define DVFUNCTION_0(MethodName, MethodPtr) \
        { \
            auto method = std::make_shared<DecaVerseCore::Method>( \
                #MethodName, \
                GenerateTypeId(typeid(decltype(MethodPtr)).name()), \
                DecaVerseCore::Method::CreateInvoker(MethodPtr) \
            ); \
            ThisClass::StaticType()->AddMethod(method); \
        }

// 声明并注册方法 (UE风格) - 1个参数
#define DVFUNCTION_1(MethodName, MethodPtr) \
        { \
            auto method = std::make_shared<DecaVerseCore::Method>( \
                #MethodName, \
                GenerateTypeId(typeid(decltype(MethodPtr)).name()), \
                DecaVerseCore::Method::CreateInvoker(MethodPtr) \
            ); \
            ThisClass::StaticType()->AddMethod(method); \
        }

// 声明并注册方法 (UE风格) - 2个参数
#define DVFUNCTION_2(MethodName, MethodPtr) \
        { \
            auto method = std::make_shared<DecaVerseCore::Method>( \
                #MethodName, \
                GenerateTypeId(typeid(decltype(MethodPtr)).name()), \
                DecaVerseCore::Method::CreateInvoker(MethodPtr) \
            ); \
            ThisClass::StaticType()->AddMethod(method); \
        }

// 结束反射声明块 - 生成静态注册函数的结束
#define DVREFLECT_END() \
    }

// ========================================
// 增强版实现宏 - 自动调用 RegisterReflection
// ========================================

// 在 CPP 文件中实现类型注册（带父类）- 自动调用反射注册
#define DECAVERSE_IMPLEMENT_TYPE_AUTO(ClassName, ParentClass) \
    DecaVerseCore::Type* ClassName::StaticType() \
    { \
        static std::shared_ptr<DecaVerseCore::Type> type = nullptr; \
        if (!type) \
        { \
            type = std::make_shared<DecaVerseCore::Type>( \
                #ClassName, \
                GenerateTypeId(#ClassName), \
                sizeof(ClassName), \
                ParentClass::StaticType() \
            ); \
            type->SetFactory(&ClassName::CreateInstance); \
            DecaVerseCore::TypeRegistry::GetInstance().RegisterType(type); \
            ClassName::RegisterReflection(); \
        } \
        return type.get(); \
    }

// 在 CPP 文件中实现类型注册（无父类）- 自动调用反射注册
#define DECAVERSE_IMPLEMENT_TYPE_ROOT_AUTO(ClassName) \
    DecaVerseCore::Type* ClassName::StaticType() \
    { \
        static std::shared_ptr<DecaVerseCore::Type> type = nullptr; \
        if (!type) \
        { \
            type = std::make_shared<DecaVerseCore::Type>( \
                #ClassName, \
                GenerateTypeId(#ClassName), \
                sizeof(ClassName), \
                nullptr \
            ); \
            type->SetFactory(&ClassName::CreateInstance); \
            DecaVerseCore::TypeRegistry::GetInstance().RegisterType(type); \
            ClassName::RegisterReflection(); \
        } \
        return type.get(); \
    }
