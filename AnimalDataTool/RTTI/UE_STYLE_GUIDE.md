# UE风格反射宏使用指南

## 概述

新增了类似Unreal Engine的UPROPERTY和UFUNCTION风格的反射宏，使得属性和方法的声明与注册更加直观和集中。

## 新旧对比

### 旧方式 (手动RegisterReflection)

```cpp
class DVObject
{
    DECAVERSE_TYPE_ROOT(DVObject)

private:
    int m_id;
    std::string m_name;

public:
    void Update();

    static void RegisterReflection()
    {
        DECAVERSE_PROPERTY(DVObject, id, m_id)
        DECAVERSE_PROPERTY(DVObject, name, m_name)
        DECAVERSE_METHOD_0(DVObject, Update, &DVObject::Update)
    }
};

// 在CPP文件中
DECAVERSE_IMPLEMENT_TYPE_ROOT(DVObject)

// 然后需要手动调用
DVObject::RegisterReflection();
```

**缺点：**
- 反射信息与成员声明分离
- 需要重复写类名
- 容易忘记调用RegisterReflection()
- 维护时容易出错（添加成员忘记注册）

### 新方式 (UE风格)

```cpp
class DVObject
{
    DECAVERSE_TYPE_ROOT(DVObject)

    // 反射声明块 - 声明即注册
    DVREFLECT_BEGIN(DVObject)
        DVPROPERTY(int, id, m_id)
        DVPROPERTY(std::string, name, m_name)
        DVFUNCTION_0(Update, &DVObject::Update)
    DVREFLECT_END()

private:
    int m_id;
    std::string m_name;

public:
    void Update();
};

// 在CPP文件中 - 自动调用RegisterReflection
DECAVERSE_IMPLEMENT_TYPE_ROOT_AUTO(DVObject)
```

**优点：**
- ✅ 反射信息集中在类声明中
- ✅ 类名只需要写一次
- ✅ 自动注册，无需手动调用
- ✅ 类似UE的UPROPERTY/UFUNCTION，直观易懂
- ✅ 更容易维护和发现错误

## 新增宏详解

### 1. DVREFLECT_BEGIN(ClassName)

开始反射声明块，自动生成`RegisterReflection()`静态函数的开始部分。

**用法：**
```cpp
class MyClass
{
    DECAVERSE_TYPE_ROOT(MyClass)

    DVREFLECT_BEGIN(MyClass)
        // 在这里声明需要反射的属性和方法
    DVREFLECT_END()
};
```

### 2. DVPROPERTY(Type, PropertyName, MemberVariable)

声明并注册一个属性。

**参数：**
- `Type`: 属性的C++类型（如`int`, `float`, `std::string`）
- `PropertyName`: 属性在反射系统中的名称（字符串）
- `MemberVariable`: 类的成员变量名

**示例：**
```cpp
DVREFLECT_BEGIN(Player)
    DVPROPERTY(int, health, m_health)
    DVPROPERTY(std::string, playerName, m_name)
    DVPROPERTY(float, speed, m_moveSpeed)
DVREFLECT_END()

private:
    int m_health;
    std::string m_name;
    float m_moveSpeed;
```

### 3. DVFUNCTION_0/1/2(MethodName, MethodPointer)

声明并注册方法，根据参数数量选择对应的宏。

**参数：**
- `MethodName`: 方法在反射系统中的名称
- `MethodPointer`: 方法指针（&ClassName::MethodName）

**DVFUNCTION_0**: 无参数方法
```cpp
DVFUNCTION_0(Update, &MyClass::Update)

void Update(); // 无参数
```

**DVFUNCTION_1**: 1个参数的方法
```cpp
DVFUNCTION_1(SetValue, &MyClass::SetValue)

void SetValue(int value); // 1个参数
```

**DVFUNCTION_2**: 2个参数的方法
```cpp
DVFUNCTION_2(Move, &MyClass::Move)

void Move(float x, float y); // 2个参数
```

### 4. DVREFLECT_END()

结束反射声明块，自动生成`RegisterReflection()`静态函数的结束部分。

### 5. DECAVERSE_IMPLEMENT_TYPE_ROOT_AUTO(ClassName)

在CPP文件中实现类型注册（根类），**自动调用**`RegisterReflection()`。

**用法：**
```cpp
// 在.cpp文件中
DECAVERSE_IMPLEMENT_TYPE_ROOT_AUTO(MyClass)
```

### 6. DECAVERSE_IMPLEMENT_TYPE_AUTO(ClassName, ParentClass)

在CPP文件中实现类型注册（派生类），**自动调用**`RegisterReflection()`。

**用法：**
```cpp
// 在.cpp文件中
DECAVERSE_IMPLEMENT_TYPE_AUTO(DerivedClass, BaseClass)
```

## 完整示例

### 头文件 (MyGame.h)

```cpp
#pragma once
#include "Core/Base/public/DecaVerseRTTI.h"

// 基类 - GameObject
class GameObject
{
    DECAVERSE_TYPE_ROOT(GameObject)

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
    virtual ~GameObject() = default;

    void PrintInfo();
};

// 派生类 - Player
class Player : public GameObject
{
    DECAVERSE_TYPE(Player, GameObject)

    DVREFLECT_BEGIN(Player)
        DVPROPERTY(float, health, m_health)
        DVPROPERTY(int, level, m_level)
        DVFUNCTION_1(TakeDamage, &Player::TakeDamage)
        DVFUNCTION_2(MoveTo, &Player::MoveTo)
    DVREFLECT_END()

private:
    float m_health = 100.0f;
    int m_level = 1;

public:
    Player() = default;

    void TakeDamage(float damage);
    void MoveTo(float x, float y);
};
```

### 实现文件 (MyGame.cpp)

```cpp
#include "MyGame.h"
#include <iostream>

using namespace DecaVerseCore;

// 实现类型注册 - 自动调用RegisterReflection
DECAVERSE_IMPLEMENT_TYPE_ROOT_AUTO(GameObject)
DECAVERSE_IMPLEMENT_TYPE_AUTO(Player, GameObject)

void GameObject::PrintInfo()
{
    std::cout << "GameObject: id=" << m_id
              << ", name=" << m_name << std::endl;
}

void Player::TakeDamage(float damage)
{
    m_health -= damage;
    std::cout << "Player took " << damage
              << " damage. Health: " << m_health << std::endl;
}

void Player::MoveTo(float x, float y)
{
    std::cout << "Player moving to (" << x << ", " << y << ")" << std::endl;
}
```

### 使用示例 (main.cpp)

```cpp
#include "MyGame.h"
#include <iostream>

using namespace DecaVerse;
using namespace DecaVerseCore;

int main()
{
    // 1. 获取类型信息
    Type* playerType = Player::StaticType();
    std::cout << "Type: " << playerType->GetName() << "\n";

    // 2. 动态创建对象
    Player* player = CreateInstance<Player>("Player");

    // 3. 属性反射 - Set
    Property* healthProp = playerType->GetProperty("health");
    healthProp->SetValue(player, Any(80.0f));

    Property* levelProp = playerType->GetProperty("level");
    levelProp->SetValue(player, Any(5));

    // 4. 属性反射 - Get
    Any healthValue = healthProp->GetValue(player);
    const float* health = healthValue.As<float>();
    std::cout << "Health: " << *health << "\n";

    // 5. 方法反射 - 调用
    Method* takeDamageMethod = playerType->GetMethod("TakeDamage");
    std::vector<Any> args = { Any(25.0f) };
    takeDamageMethod->Invoke(player, args);

    Method* moveMethod = playerType->GetMethod("MoveTo");
    args = { Any(10.0f), Any(20.0f) };
    moveMethod->Invoke(player, args);

    delete player;
    return 0;
}
```

## 对比总结

| 特性 | 旧方式 | UE风格新方式 |
|------|--------|--------------|
| 反射声明位置 | RegisterReflection函数中 | DVREFLECT_BEGIN/END块中 |
| 类名重复 | 每个属性/方法都需要 | 只在DVREFLECT_BEGIN中写一次 |
| 自动注册 | ❌ 需手动调用 | ✅ IMPLEMENT_TYPE_AUTO自动调用 |
| 代码可读性 | 中等 | 高 - 类似UE风格 |
| 维护难度 | 较高 | 较低 |
| 学习曲线 | 需要理解注册流程 | 类似UE，更直观 |

## 何时使用哪种方式？

### 使用UE风格（推荐）

- ✅ 新项目或新类
- ✅ 需要频繁添加/修改属性和方法
- ✅ 团队熟悉UE开发模式
- ✅ 追求代码整洁和可维护性

### 使用旧方式

- ⚠️ 已有大量代码使用旧方式
- ⚠️ 需要运行时动态注册（RegisterReflection可以随时调用）
- ⚠️ 需要条件注册（某些情况下不注册某些属性）

## 注意事项

1. **DVREFLECT_BEGIN/END必须成对使用**
   ```cpp
   DVREFLECT_BEGIN(MyClass)
       // 反射声明
   DVREFLECT_END()  // 不要忘记END!
   ```

2. **使用AUTO版本的IMPLEMENT宏**
   - `DECAVERSE_IMPLEMENT_TYPE_ROOT_AUTO` (根类)
   - `DECAVERSE_IMPLEMENT_TYPE_AUTO` (派生类)

   不使用AUTO版本则需要手动调用RegisterReflection()。

3. **属性类型必须精确**
   ```cpp
   DVPROPERTY(int, health, m_health)     // ✅ 正确
   DVPROPERTY(float, health, m_health)   // ❌ 错误 - 如果m_health是int
   ```

4. **方法参数数量匹配**
   ```cpp
   void Update();                         // 使用 DVFUNCTION_0
   void SetValue(int v);                  // 使用 DVFUNCTION_1
   void Move(float x, float y);           // 使用 DVFUNCTION_2
   ```

5. **两种风格可以混用**
   - 旧代码保持不变
   - 新代码使用UE风格
   - 同一个项目中可以同时存在两种风格

## 运行示例

编译并运行完整示例：

```bash
# 查看示例代码
cat RTTI/RTTI/UEStyleExample.cpp

# 编译 (假设使用Visual Studio)
msbuild RTTI.sln /p:Configuration=Release

# 运行
./bin/Release/RTTI.exe
```

## 总结

UE风格的反射宏提供了更简洁、更直观的反射系统使用方式：

- 🎯 **声明即注册** - 在类中直接声明反射信息
- 🔄 **自动化** - 无需手动调用注册函数
- 📝 **可读性高** - 类似UE的UPROPERTY/UFUNCTION
- 🛠️ **易维护** - 反射信息与成员声明更接近

推荐在新项目中使用UE风格的反射宏！
