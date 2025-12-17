#pragma once
#include <iostream>
#include "../../../Base/public/Method.h"
#include "../../../Base/public/Property.h"
#include "../../../Base/public/RTTIMacros.h"
class DVObject
{
    DECAVERSE_TYPE_ROOT(DVObject)
    DVREFLECT_BEGIN(DVObject)
        DVPROPERTY(int, id, m_id)
        DVPROPERTY(std::string,name, m_name)
    DVREFLECT_END()
public:
 
public:
    // 在类内部注册反射信息
    
    DVObject() : m_id(0), m_name("DVObject") {}
    virtual ~DVObject() = default;

    int GetId() const { return m_id; }
    void SetId(int id) { m_id = id; }

    const std::string& GetName() const { return m_name; }
    void SetName(const std::string& name) { m_name = name; }

    virtual void Update()
    {
        std::cout << "DVObject::Update() - " << m_name << '\n';
    }

protected:
    int m_id;
    std::string m_name;
};
