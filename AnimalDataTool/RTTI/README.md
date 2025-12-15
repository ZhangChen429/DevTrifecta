# DecaVerse RTTI Framework

DecaVerse 游戏引擎的运行时类型信息（RTTI）框架，提供强大的反射能力。

## 核心特性

- **类型信息管理** - 完整的类型元数据支持
- **继承关系检查** - 运行时类型判断和继承关系查询
- **属性反射** - 动态读写对象属性
- **方法反射** - 动态调用对象方法
- **类型注册表** - 全局类型管理和查询
- **对象工厂** - 通过类型名称动态创建对象
- **线程安全** - 类型注册表使用互斥锁保证线程安全

## 文件结构

```
RTTI/Core/Base/
├── public/
│   ├── DecaVerseTypeId.h      - 类型 ID 定义和生成
│   ├── Type.h                 - 核心类型信息类
│   ├── Property.h             - 属性反射系统
│   ├── Method.h               - 方法反射系统
│   ├── TypeRegistry.h         - 全局类型注册表
│   ├── RTTIMacros.h           - 便捷宏定义
│   └── DecaVerseRTTI.h        - 主头文件（包含所有）
└── private/
    ├── DecaVerseTypeId.cpp    - TypeId 生成实现
    └── Type.cpp               - Type 类实现
```

## 快速开始

### 1. 包含头文件

```cpp
#include "Core/Base/public/DecaVerseRTTI.h"
using namespace DecaVerseCore;
```

### 2. 声明类型（在头文件中）

```cpp
// 基类
class GameObject
{
    DECAVERSE_TYPE_ROOT(GameObject)  // 根类使用 TYPE_ROOT

public:
    GameObject() : m_id(0), m_name("GameObject") {}
    virtual ~GameObject() = default;

protected:
    int m_id;
    std::string m_name;
};

// 派生类
class Actor : public GameObject
{
    DECAVERSE_TYPE(Actor, GameObject)  // 派生类指定父类

public:
    Actor() : m_health(100.0f) {}

    void TakeDamage(float damage) { m_health -= damage; }

private:
    float m_health;
};
```

### 3. 实现类型注册（在 CPP 文件中）

```cpp
// 实现类型
DECAVERSE_IMPLEMENT_TYPE_ROOT(GameObject)
DECAVERSE_IMPLEMENT_TYPE(Actor, GameObject)

// 注册属性和方法
void RegisterReflection()
{
    // GameObject 反射
    DECAVERSE_PROPERTY(GameObject, id, m_id);
    DECAVERSE_PROPERTY(GameObject, name, m_name);

    // Actor 反射
    DECAVERSE_PROPERTY(Actor, health, m_health);
    DECAVERSE_METHOD_1(Actor, TakeDamage, &Actor::TakeDamage);
}
```

### 4. 使用反射

```cpp
int main()
{
    // 注册反射信息
    RegisterReflection();

    // 1. 获取类型信息
    Type* actorType = Actor::StaticType();
    std::cout << "Type: " << actorType->GetName() << std::endl;
    std::cout << "Size: " << actorType->GetSize() << " bytes" << std::endl;

    // 2. 检查继承关系
    if (actorType->IsA("GameObject"))
        std::cout << "Actor is a GameObject" << std::endl;

    // 3. 动态创建对象
    Actor* actor = DecaVerse::CreateInstance<Actor>("Actor");

    // 4. 属性反射 - 读取
    Property* healthProp = actorType->GetProperty("health");
    Any healthValue = healthProp->GetValue(actor);
    const float* health = healthValue.As<float>();
    std::cout << "Health: " << *health << std::endl;

    // 5. 属性反射 - 设置
    healthProp->SetValue(actor, Any(75.0f));

    // 6. 方法反射 - 调用
    Method* takeDamageMethod = actorType->GetMethod("TakeDamage");
    std::vector<Any> args = { Any(30.0f) };
    takeDamageMethod->Invoke(actor, args);

    // 7. 遍历所有属性
    for (const auto& prop : actorType->GetProperties())
        std::cout << "Property: " << prop->GetName() << std::endl;

    // 8. 查询类型注册表
    auto& registry = TypeRegistry::GetInstance();
    std::cout << "Total types: " << registry.GetTypeCount() << std::endl;

    delete actor;
    return 0;
}
```

## API 参考

### 核心宏

| 宏 | 用途 | 使用位置 |
|----|------|----------|
| `DECAVERSE_TYPE_ROOT(ClassName)` | 声明根类的 RTTI 支持 | 类定义 |
| `DECAVERSE_TYPE(ClassName, ParentClass)` | 声明派生类的 RTTI 支持 | 类定义 |
| `DECAVERSE_IMPLEMENT_TYPE_ROOT(ClassName)` | 实现根类的类型注册 | CPP 文件 |
| `DECAVERSE_IMPLEMENT_TYPE(ClassName, ParentClass)` | 实现派生类的类型注册 | CPP 文件 |
| `DECAVERSE_PROPERTY(ClassName, PropName, MemberVar)` | 注册属性 | 注册函数 |
| `DECAVERSE_METHOD_0(ClassName, MethodName, MethodPtr)` | 注册无参方法 | 注册函数 |
| `DECAVERSE_METHOD_1(ClassName, MethodName, MethodPtr)` | 注册单参数方法 | 注册函数 |
| `DECAVERSE_METHOD_2(ClassName, MethodName, MethodPtr)` | 注册双参数方法 | 注册函数 |

### Type 类

```cpp
class Type
{
    // 基本信息
    const std::string& GetName() const;
    TypeId GetTypeId() const;
    size_t GetSize() const;
    Type* GetParent() const;

    // 继承检查
    bool IsA(TypeId typeId) const;
    bool IsA(const std::string& typeName) const;
    bool IsChildOf(const Type* type) const;

    // 属性管理
    Property* GetProperty(const std::string& name) const;
    const std::vector<std::shared_ptr<Property>>& GetProperties() const;

    // 方法管理
    Method* GetMethod(const std::string& name) const;
    const std::vector<std::shared_ptr<Method>>& GetMethods() const;

    // 对象创建
    void* CreateInstance() const;
};
```

### Property 类

```cpp
class Property
{
    const std::string& GetName() const;
    TypeId GetTypeId() const;
    size_t GetOffset() const;

    Any GetValue(void* instance) const;
    void SetValue(void* instance, const Any& value) const;
};
```

### Method 类

```cpp
class Method
{
    const std::string& GetName() const;
    TypeId GetReturnTypeId() const;

    Any Invoke(void* instance, const std::vector<Any>& args = {}) const;
};
```

### TypeRegistry 类

```cpp
class TypeRegistry
{
    static TypeRegistry& GetInstance();

    void RegisterType(std::shared_ptr<Type> type);
    Type* FindTypeByName(const std::string& name) const;
    Type* FindTypeById(TypeId id) const;
    std::vector<Type*> GetAllTypes() const;
    size_t GetTypeCount() const;
};
```

### Any 类

用于存储和传递任意类型的值：

```cpp
class Any
{
    template<typename T> Any(const T& value);

    template<typename T> T* As();
    template<typename T> const T* As() const;

    TypeId GetTypeId() const;
    bool IsValid() const;
};
```

## 使用场景

1. **序列化/反序列化** - 使用属性反射读写对象数据
2. **编辑器工具** - 动态显示和编辑对象属性
3. **脚本绑定** - 将 C++ 类导出到脚本语言
4. **网络同步** - 动态复制对象状态
5. **配置系统** - 从配置文件创建和初始化对象
6. **调试工具** - 运行时检查对象类型和属性
7. **插件系统** - 动态加载和创建对象

## 技术细节

### TypeId 生成

使用 FNV-1a 哈希算法生成 64 位类型 ID，确保：
- 高性能（编译期可计算）
- 低碰撞率
- 跨平台一致性

### 线程安全

TypeRegistry 使用 `std::mutex` 保证多线程环境下的安全性。

### 内存管理

- 类型信息使用 `shared_ptr` 自动管理
- 属性和方法使用 `shared_ptr` 避免手动释放
- 对象创建后需要用户手动管理生命周期

## 扩展建议

1. **增强 Any 类型** - 支持引用和右值引用
2. **添加序列化支持** - 自动序列化/反序列化
3. **支持更多参数的方法** - 目前支持 0-2 个参数
4. **添加属性元数据** - 如默认值、范围、描述等
5. **支持枚举反射** - 枚举类型的名称-值映射
6. **添加类型转换** - 安全的类型转换工具

## 示例程序

完整的示例程序在 `RTTI.cpp` 中，包含：
- GameObject 基类和 Actor 派生类
- 完整的属性和方法反射
- 8 个测试用例展示所有功能

编译并运行程序查看完整演示。

## 许可证

DecaVerse Game Engine RTTI Framework
