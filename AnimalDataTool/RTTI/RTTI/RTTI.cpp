
#include <iostream>
#include <typeinfo>
#include <exception>
#include "Core/Base/public/DecaVerseRTTI.h"

using namespace DecaVerseCore;
using namespace DecaVerse;

// ============================================================================
// 示例类：GameObject 基类
// ============================================================================
class GameObject
{
    DECAVERSE_TYPE_ROOT(GameObject)

public:
    // 在类内部注册反射信息
    static void RegisterReflection()
    {
        DECAVERSE_PROPERTY(GameObject, id, m_id)
        DECAVERSE_PROPERTY(GameObject, name, m_name)
        DECAVERSE_METHOD_0(GameObject, Update, &GameObject::Update)
    }
    GameObject() : m_id(0), m_name("GameObject") {}
    virtual ~GameObject() = default;

    int GetId() const { return m_id; }
    void SetId(int id) { m_id = id; }

    const std::string& GetName() const { return m_name; }
    void SetName(const std::string& name) { m_name = name; }

    virtual void Update()
    {
        std::cout << "GameObject::Update() - " << m_name << '\n';
    }

protected:
    int m_id;
    std::string m_name;
};

// ============================================================================
// 示例类：Actor 继承自 GameObject
// ============================================================================
class Actor : public GameObject
{
    DECAVERSE_TYPE(Actor, GameObject)

public:
    // 在类内部注册反射信息
    static void RegisterReflection()
    {
        // ✅ 类的静态成员函数可以访问私有成员
        DECAVERSE_PROPERTY(Actor, health, m_health)
        DECAVERSE_PROPERTY(Actor, speed, m_speed)
        DECAVERSE_METHOD_0(Actor, Update, &Actor::Update)
        DECAVERSE_METHOD_1(Actor, TakeDamage, &Actor::TakeDamage)
        DECAVERSE_METHOD_1(Actor, Move, &Actor::Move)
    }
    Actor() : m_health(100.0f), m_speed(5.0f)
    {
        m_name = "Actor";
    }

    float GetHealth() const { return m_health; }
    void SetHealth(float health) { m_health = health; }

    float GetSpeed() const { return m_speed; }
    void SetSpeed(float speed) { m_speed = speed; }

    void TakeDamage(float damage)
    {
        m_health -= damage;
        std::cout << m_name << " took " << damage << " damage. Health: " << m_health << '\n';
    }

    void Move(float distance)
    {
        std::cout << m_name << " moved " << distance << " units at speed " << m_speed << '\n';
    }

    void Update() override
    {
        std::cout << "Actor::Update() - " << m_name << " (Health: " << m_health << ")" << '\n';
    }

private:
    float m_health;
    float m_speed;
};

// ============================================================================
// 类型注册实现
// ============================================================================
DECAVERSE_IMPLEMENT_TYPE_ROOT(GameObject)
DECAVERSE_IMPLEMENT_TYPE(Actor, GameObject)



// ============================================================================
// 测试函数
// ============================================================================
void TestRTTI()
{
    std::cout << "\n========================================" << '\n';
    std::cout << "DecaVerse RTTI Framework Test" << '\n';
    std::cout << "========================================\n" << '\n';

    // 1. 类型信息测试
    std::cout << "--- 1. Type Information Test ---" << '\n';
    const Type* gameObjectType = GameObject::StaticType();
    const Type* actorType = Actor::StaticType();

    std::cout << "GameObject Type: " << gameObjectType->GetName()
              << ", Size: " << gameObjectType->GetSize() << " bytes" << '\n';
    std::cout << "Actor Type: " << actorType->GetName()
              << ", Size: " << actorType->GetSize() << " bytes" << '\n';

    // 2. 继承关系测试
    std::cout << "\n--- 2. Inheritance Test ---" << '\n';
    std::cout << "Actor IsA GameObject: " << (actorType->IsA("GameObject") ? "Yes" : "No") << '\n';
    std::cout << "GameObject IsA Actor: " << (gameObjectType->IsA("Actor") ? "Yes" : "No") << '\n';

    // 3. 对象创建测试
    std::cout << "\n--- 3. Object Creation Test ---" << '\n';
    Actor* actor = CreateInstance<Actor>("Actor");
    if (actor)
    {
        std::cout << "Successfully created Actor instance" <<'\n';
        actor->SetName("Hero");
        actor->SetId(1001);
        std::cout << "Actor Name: " << actor->GetName() << ", ID: " << actor->GetId() << '\n';
    }

    // 4. 属性反射测试
    std::cout << "\n--- 4. Property Reflection Test ---" << '\n';
    if (actor)
    {
        const Property* healthProp = actorType->GetProperty("health");
        if (healthProp)
        {
            // 读取属性
            Any healthValue = healthProp->GetValue(actor);
            const float* health = healthValue.As<float>();
            if (health)
                std::cout << "Health (via reflection): " << *health <<'\n';

            // 设置属性
            healthProp->SetValue(actor, Any(75.0f));
            std::cout << "Health after SetValue: " << actor->GetHealth() << '\n';
        }

        const Property* nameProp = actorType->GetProperty("name");
        if (nameProp)
        {
            nameProp->SetValue(actor, Any(std::string("Warrior")));
            std::cout << "Name after SetValue: " << actor->GetName() << '\n';
        }
    }

    // 5. 方法反射测试
    std::cout << "\n--- 5. Method Reflection Test ---" <<'\n';
    if (actor)
    {
        const Method* updateMethod = actorType->GetMethod("Update");
        if (updateMethod)
        {
            std::cout << "Calling Update via reflection:" << '\n';
            updateMethod->Invoke(actor);
        }

        const Method* takeDamageMethod = actorType->GetMethod("TakeDamage");
        if (takeDamageMethod)
        {
            std::cout << "Calling TakeDamage(30.0) via reflection:" << '\n';
            const std::vector<Any> args = { Any(30.0f) };
            takeDamageMethod->Invoke(actor, args);
        }

        const Method* moveMethod = actorType->GetMethod("Move");
        if (moveMethod)
        {
            std::cout << "Calling Move(10.5) via reflection:" << '\n';
            std::vector<Any> args = { Any(10.5f) };
            moveMethod->Invoke(actor, args);
        }
    }

    // 6. 类型注册表测试
    std::cout << "\n--- 6. Type Registry Test ---" << '\n';
    const auto& registry = TypeRegistry::GetInstance();
    std::cout << "Total registered types: " << registry.GetTypeCount() << '\n';

    std::cout << "All registered types:" << '\n';
    for (Type* type : registry.GetAllTypes())
    {
        std::cout << "  - " << type->GetName()
                  << " (ID: " << type->GetTypeId()
                  << ", Size: " << type->GetSize() << " bytes)" << '\n';
    }

    // 7. 属性遍历测试
    std::cout << "\n--- 7. Property Enumeration Test ---" << '\n';
    std::cout << "Actor properties:" << '\n';
    for (const auto& prop : actorType->GetProperties())
    {
        std::cout << "  - " << prop->GetName() << " (Offset: " << prop->GetOffset() << ")" <<'\n';
    }

    // 8. 方法遍历测试
    std::cout << "\n--- 8. Method Enumeration Test ---" <<'\n';
    std::cout << "Actor methods:" << '\n';
    for (const auto& method : actorType->GetMethods())
    {
        std::cout << "  - " << method->GetName() << '\n';
    }

    // 清理
    delete actor;

    std::cout << "\n========================================" << '\n';
    std::cout << "Test Completed!" << '\n';
    std::cout << "========================================\n" << '\n';
}

// ============================================================================
// 主函数
// ============================================================================
int main(int argc, char* argv[])
{
    std::cout << "DecaVerse Game Engine - RTTI Framework Demo" << '\n';
    std::cout << "============================================\n" <<'\n';

    // 注册反射信息
    GameObject::RegisterReflection();
    Actor::RegisterReflection();

    // 运行测试
    TestRTTI();

    std::cout << "\nPress Enter to exit...";
    std::cin.get();

    return 0;
}
