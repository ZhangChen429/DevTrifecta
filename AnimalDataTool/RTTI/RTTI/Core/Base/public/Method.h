#pragma once
#include "Property.h"
#include <string>
#include <vector>
#include <functional>

namespace DecaVerseCore
{
    // Method 类 - 方法反射
    class Method
    {
    public:
        using InvokeFunction = std::function<Any(void*, const std::vector<Any>&)>;

        Method(const std::string& name, TypeId returnTypeId, InvokeFunction invoker)
            : m_name(name)
            , m_returnTypeId(returnTypeId)
            , m_invoker(invoker)
        {
        }

        const std::string& GetName() const { return m_name; }
        TypeId GetReturnTypeId() const { return m_returnTypeId; }

        Any Invoke(void* instance, const std::vector<Any>& args = {}) const
        {
            if (m_invoker)
                return m_invoker(instance, args);
            return Any();
        }

        // 无参数方法
        template<typename ClassType, typename ReturnType>
        static InvokeFunction CreateInvoker(ReturnType(ClassType::*methodPtr)())
        {
            return [methodPtr](void* instance, const std::vector<Any>&) -> Any
            {
                ClassType* obj = static_cast<ClassType*>(instance);
                if constexpr (std::is_void_v<ReturnType>)
                {
                    (obj->*methodPtr)();
                    return Any();
                }
                else
                {
                    return Any((obj->*methodPtr)());
                }
            };
        }

        // 单参数方法
        template<typename ClassType, typename ReturnType, typename Arg1>
        static InvokeFunction CreateInvoker(ReturnType(ClassType::*methodPtr)(Arg1))
        {
            return [methodPtr](void* instance, const std::vector<Any>& args) -> Any
            {
                if (args.size() < 1)
                    return Any();

                ClassType* obj = static_cast<ClassType*>(instance);
                const Arg1* arg1 = args[0].As<Arg1>();
                if (!arg1)
                    return Any();

                if constexpr (std::is_void_v<ReturnType>)
                {
                    (obj->*methodPtr)(*arg1);
                    return Any();
                }
                else
                {
                    return Any((obj->*methodPtr)(*arg1));
                }
            };
        }

        // 双参数方法
        template<typename ClassType, typename ReturnType, typename Arg1, typename Arg2>
        static InvokeFunction CreateInvoker(ReturnType(ClassType::*methodPtr)(Arg1, Arg2))
        {
            return [methodPtr](void* instance, const std::vector<Any>& args) -> Any
            {
                if (args.size() < 2)
                    return Any();

                ClassType* obj = static_cast<ClassType*>(instance);
                const Arg1* arg1 = args[0].As<Arg1>();
                const Arg2* arg2 = args[1].As<Arg2>();
                if (!arg1 || !arg2)
                    return Any();

                if constexpr (std::is_void_v<ReturnType>)
                {
                    (obj->*methodPtr)(*arg1, *arg2);
                    return Any();
                }
                else
                {
                    return Any((obj->*methodPtr)(*arg1, *arg2));
                }
            };
        }

    private:
        std::string m_name;
        TypeId m_returnTypeId;
        InvokeFunction m_invoker;
    };
}
