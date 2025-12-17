// UE风格反射宏使用示例
#include "Core/Base/public/DecaVerseRTTI.h"
#include <iostream>

using namespace DecaVerse;
using namespace DecaVerseCore;

// ========================================
// 示例类定义 - 使用 UE 风格的反射宏
// ========================================

// 基类 - GameObject
class GameObject
{
    DECAVERSE_TYPE_ROOT(GameObject)

    // UE风格的反射声明块
    DVREFLECT_BEGIN(GameObject)
        DVPROPERTY(int, id, m_id)
        DVPROPERTY(std::string, name, m_name)
        DVFUNCTION_0(PrintInfo, &GameObject::PrintInfo)
    DVREFLECT_END()

private:
    int m_id = 0;
    std::string m_name = "GameObject";

public:
    GameObject() = default;
    GameObject(int id, const std::string& name) : m_id(id), m_name(name) {}
    virtual ~GameObject() = default;

    void PrintInfo()
    {
        std::cout << "GameObject: id=" << m_id << ", name=" << m_name << std::endl;
    }
};

// 派生类 - Actor
class Actor : public GameObject
{
    DECAVERSE_TYPE(Actor, GameObject)

    // UE风格的反射声明块
    DVREFLECT_BEGIN(Actor)
        DVPROPERTY(float, health, m_health)
        DVPROPERTY(bool, isAlive, m_isAlive)
        DVFUNCTION_1(TakeDamage, &Actor::TakeDamage)
        DVFUNCTION_0(Die, &Actor::Die)
    DVREFLECT_END()

private:
    float m_health = 100.0f;
    bool m_isAlive = true;

public:
    Actor() = default;
    Actor(int id, const std::string& name, float health)
        : GameObject(id, name), m_health(health), m_isAlive(true)
    {
    }

    void TakeDamage(float damage)
    {
        m_health -= damage;
        std::cout << "Actor took " << damage << " damage. Health: " << m_health << std::endl;
        if (m_health <= 0.0f)
        {
            Die();
        }
    }

    void Die()
    {
        m_isAlive = false;
        std::cout << "Actor died!" << std::endl;
    }
};

// ========================================
// 实现类型注册 - 使用自动反射注册宏
// ========================================

DECAVERSE_IMPLEMENT_TYPE_ROOT_AUTO(GameObject)
DECAVERSE_IMPLEMENT_TYPE_AUTO(Actor, GameObject)

// ========================================
// 主函数 - 演示 UE 风格的 RTTI 使用
// ========================================

int main()
{
    std::cout << "========================================\n";
    std::cout << "UE Style RTTI Example\n";
    std::cout << "========================================\n\n";

    // 1. 获取类型信息
    std::cout << "=== 1. Type Information ===\n";
    Type* gameObjectType = GameObject::StaticType();
    Type* actorType = Actor::StaticType();

    std::cout << "GameObject Type: " << gameObjectType->GetName() << "\n";
    std::cout << "Actor Type: " << actorType->GetName() << "\n";
    std::cout << "Actor's Parent: " << actorType->GetParent()->GetName() << "\n\n";

    // 2. 检查继承关系
    std::cout << "=== 2. Inheritance Check ===\n";
    std::cout << "Actor IsChildOf GameObject: "
              << (actorType->IsChildOf(gameObjectType) ? "true" : "false") << "\n";
    std::cout << "Actor IsA GameObject: "
              << (actorType->IsA(gameObjectType->GetTypeId()) ? "true" : "false") << "\n\n";

    // 3. 动态创建对象
    std::cout << "=== 3. Dynamic Object Creation ===\n";
    Actor* actor = CreateInstance<Actor>("Actor");
    if (actor)
    {
        std::cout << "Actor instance created successfully!\n\n";

        // 4. 属性反射 - Get
        std::cout << "=== 4. Property Reflection - Get ===\n";
        Property* healthProp = actorType->GetProperty("health");
        if (healthProp)
        {
            Any healthValue = healthProp->GetValue(actor);
            const float* health = healthValue.As<float>();
            if (health)
            {
                std::cout << "health = " << *health << "\n";
            }
        }

        Property* isAliveProp = actorType->GetProperty("isAlive");
        if (isAliveProp)
        {
            Any isAliveValue = isAliveProp->GetValue(actor);
            const bool* isAlive = isAliveValue.As<bool>();
            if (isAlive)
            {
                std::cout << "isAlive = " << (*isAlive ? "true" : "false") << "\n\n";
            }
        }

        // 5. 属性反射 - Set
        std::cout << "=== 5. Property Reflection - Set ===\n";
        Property* idProp = actorType->GetProperty("id");
        Property* nameProp = actorType->GetProperty("name");

        if (idProp)
        {
            idProp->SetValue(actor, Any(42));
            std::cout << "Set id to 42\n";
        }

        if (nameProp)
        {
            nameProp->SetValue(actor, Any(std::string("Hero")));
            std::cout << "Set name to 'Hero'\n\n";
        }

        // 6. 方法反射 - 调用
        std::cout << "=== 6. Method Reflection - Invoke ===\n";
        Method* printInfoMethod = actorType->GetMethod("PrintInfo");
        if (printInfoMethod)
        {
            printInfoMethod->Invoke(actor);
        }

        Method* takeDamageMethod = actorType->GetMethod("TakeDamage");
        if (takeDamageMethod)
        {
            std::vector<Any> args;
            args.push_back(Any(30.0f));
            takeDamageMethod->Invoke(actor, args);
        }

        std::cout << "\n";

        // 7. 枚举所有属性
        std::cout << "=== 7. Enumerate All Properties ===\n";
        const auto& properties = actorType->GetProperties();
        std::cout << "Actor has " << properties.size() << " properties:\n";
        for (const auto& prop : properties)
        {
            std::cout << "  - " << prop->GetName() << "\n";
        }
        std::cout << "\n";

        // 8. 枚举所有方法
        std::cout << "=== 8. Enumerate All Methods ===\n";
        const auto& methods = actorType->GetMethods();
        std::cout << "Actor has " << methods.size() << " methods:\n";
        for (const auto& method : methods)
        {
            std::cout << "  - " << method->GetName() << "\n";
        }
        std::cout << "\n";

        // 9. 查询类型注册表
        std::cout << "=== 9. Type Registry ===\n";
        TypeRegistry& registry = TypeRegistry::GetInstance();
        std::cout << "Total registered types: " << registry.GetTypeCount() << "\n";

        auto allTypes = registry.GetAllTypes();
        std::cout << "All types:\n";
        for (Type* type : allTypes)
        {
            std::cout << "  - " << type->GetName() << "\n";
        }
        std::cout << "\n";

        delete actor;
    }

    std::cout << "========================================\n";
    std::cout << "UE Style RTTI Example Complete!\n";
    std::cout << "========================================\n";

    return 0;
}
