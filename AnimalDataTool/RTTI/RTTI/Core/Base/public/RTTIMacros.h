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
